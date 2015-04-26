// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerOvi.generated.h"

#define COLLISION_PLAYER        ECC_GameTraceChannel1  
UCLASS()
class TOWARDSTHELIGHT_API APlayerOvi : public APawn
{
	GENERATED_BODY()

public:
	enum States { STOP, RIGHT, LEFT };
	// Sets default values for this pawn's properties
	APlayerOvi();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
  void ReceiveActorBeginOverlap(AActor * OtherActor) override;
  void ReceiveActorEndOverlap(AActor * OtherActor) override;
  AActor *GetCollisionActor() { return collisionActor; }
  bool IsJumping() { return m_isJumping; }
  FVector GetLastPosition() { return lastPosition; }
  //sets right flag when key is pressed
	UFUNCTION()
		void OnStartRight();
	//clears right flag when key is released
	UFUNCTION()
		void OnStopRight();
	//sets left flag when key is pressed
	UFUNCTION()
		void OnStartLeft();
	//clears left flag when key is released
	UFUNCTION()
		void OnStopLeft();

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
  UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	  class UCapsuleComponent* CapsuleComponent;
    /** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	  class USkeletalMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	  class UArrowComponent* ArrowComponent;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MaxJumpHeight;
private:
  void CalculateOrientation();
  void CalculateGravity(float DeltaTime);
  void DoJump(float DeltaTime);
  void DoMovement(float DeltaTime, float value);
  void CheckCollision();

	bool m_right;
	bool m_left;
	bool m_isJumping;
  bool m_hasLanded;
  bool m_enabledGravity;
	States m_state;
	float m_limit;
	float m_jumpDistance;
  FVector lastPosition;
  AActor *collisionActor;
};