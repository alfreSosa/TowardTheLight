// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticEnemy.h"
#include "MobileEnemy.generated.h"

/**
 * 
 */
#define COLLISION_ENEMY    ECC_GameTraceChannel2 
class APlayerOvi;

const float DEFAULT_ENEMY_CAPSULE_RADIOUS = 45.0f;
const float DEFAULT_ENEMY_CAPSULE_HEIGHT = 95.0f;
const float PADDING_ENEMY_COLLISION_PERCENT = 0.05f;

UCLASS()
class TOWARDSTHELIGHT_API AMobileEnemy : public AStaticEnemy
{
	GENERATED_BODY()
	
  float m_timer;
  float m_totalDistance;
  float m_currentDistance;
  FVector m_rightVector;
  FVector m_rightPosition;
  FVector m_leftPosition;
  bool m_initMovement;

  float m_jumpSpeed;
  float m_accelerationJump;
  float m_actualJumpSpeed;
  float m_capsuleHeight;
  float m_capsuleRadious;
  float m_capsuleHeightPadding;
  float m_capsuleRadiousPadding;
  bool m_enableGravity;
  APlayerOvi *m_player;
  FVector m_lastPosition;
  

  enum state{
    INITIAL_DELAY,
    TO_RIGHT,
    TO_LEFT,
    RIGHT_DELAY,
    LEFT_DELAY,
  }m_state;

  void doMovement(float DeltaSeconds);
  void CalculateGravity(float DeltaSeconds);
  void CheckCollision();
  void ResponseCollision();
  void ResponseCollisionBackward();
  FVector AbsVector(const FVector& vector);
  FVector RecalculateLocation(FVector Direction, FVector Location, FVector HitLocation, float size);
public:
  UPROPERTY()
    UBoxComponent *Trigger;
  UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UCapsuleComponent* CapsuleComponent;

  AMobileEnemy();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;

  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float RightDistance;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float LeftDistance;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float RightDelay;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float LeftDelay;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float Speed;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    float InitialDelay;
  UPROPERTY(EditAnywhere, Category = MobileEnemy)
    bool HasTrigger;

  void RegisterDelegate();
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
