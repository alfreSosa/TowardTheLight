// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerOvi.generated.h"

#define COLLISION_PLAYER        ECC_GameTraceChannel1 

const float PADDING_COLLISION_PERCENT = 0.15f;
const float PADDING_COLLISION_PERCENT_FEET = 0.30f;
const float PADDING_COLLISION_PERCENT_RADIOUS = 0.55f;
const float DEFAULT_CAPSULE_RADIOUS = 30.0f;
const float CAPSULE_RADIOUS_PADDING = 5.0f;
const float DEFAULT_CAPSULE_HEIGHT = 95.0f;

const float DEFAULT_MOVEMENT_SPEED = 620.0f; //ajustado a valores de dise�o
const float DEFAULT_JUMP_SPEED = 1550.0f; //ajustado a valores de dise�o
const float DEFAULT_JUMP_ACC = 3300.0f; //ajustado a valores de dise�o

class AMobilePlatform;
class AStick;
class AMyGameMode;
UCLASS()
class TOWARDSTHELIGHT_API APlayerOvi : public APawn
{
	GENERATED_BODY()

public:
	enum States { RIGHT, LEFT, STOP};
	// Sets default values for this pawn's properties
	APlayerOvi();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  //sets right flag when key is pressed
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
		void OnStartRight();
	//clears right flag when key is released
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
		void OnStopRight();
	//sets left flag when key is pressed
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
		void OnStartLeft();
	//clears left flag when key is released
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
		void OnStopLeft();
  //sets jump flag when key is pressed
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
    void OnStartJump();
  //clears jump flag when key is released
  UFUNCTION(BlueprintCallable, Category = "PlayerInputTTL")
    void OnStopJump();

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location);

  UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	  class UCapsuleComponent* CapsuleComponent;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	  class USkeletalMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	  class UArrowComponent* ArrowComponent;
#endif

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputControl)
    float MarginInput;
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpSpeed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
    float AccelerationJump;

  void OnMobilePlatform(class AMobilePlatform *mp, FVector movement);
  
  void SetKey(bool key, FLinearColor colorKey);
  bool HasKey();
  FLinearColor GetColorKey();
  void EnabledPushButton();

  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool isPlayerRunning();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool PlayerStopRunning();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool isPlayerJumping();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool isPlayerFalling();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool PlayerHasLanded();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
	  bool PlayerisToRight();
  UFUNCTION(BlueprintCallable, Category = "PlayerLocomotion")
    bool PlayerisPushinButton();

  UFUNCTION(BlueprintCallable, Category = "PlayerState")
    bool isPlayerPaused();
private:

  float UpdateState();
  void CalculateOrientation();
  void CalculateGravity(float DeltaSeconds);
  void DoJump(float DeltaSeconds);
  void DoMovement(float DeltaSeconds, float value);
  void CheckCollision();
  void AjustPosition();
  void Rotate(const FVector& rotation);
  bool isInputEnabled();
  FVector AbsVector(const FVector& vector);
  FVector RecalculateLocation(FVector Direction, FVector Location, FVector HitLocation, float size);

  States m_state;
  AMyGameMode *m_gameMode;
  FCollisionQueryParams m_TraceParams;
	bool m_right;
	bool m_left;
  bool m_doJump;
  bool m_isJumping;
  bool m_isFalling;
  bool m_hasLanded;
  bool m_headCollision;
  bool m_enabledGravity;
  bool m_isInJumpTransition;
  bool m_isOnMobilePlatform;
  AMobilePlatform *m_currentMobile;
  int m_frameCounter;
  //animation boton
  bool m_isPushingButton;
  float m_elapsedButton;

  bool m_hasKey;
  FLinearColor m_colorKey;
  AStick* m_stick;
  bool bPlayerRunning;
	float m_limit;
  float m_actualAccJump;
  float m_actualJumpSpeed;
  float m_transitionDistance;
  float m_capsuleHeight;
  float m_capsuleRadious;
  float m_capsuleHeightPadding;
  float m_capsuleHeightPaddingFeet;
  float m_capsuleRadiousPadding;

  FVector m_lastPosition;
  FVector m_lastUpVector;
  FVector m_lastForwardVector;

  float m_limitViewPort0;
  float m_limitViewPort1;
  float m_centerTouchX;
  States m_stateInput;
  ETouchIndex::Type m_fingerIndexMovement;
  ETouchIndex::Type m_fingerIndexJump;
  ETouchIndex::Type m_fingerIndexOther;
};
