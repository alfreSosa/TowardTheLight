// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticEnemy.h"
#include "MobileEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AMobileEnemy : public AStaticEnemy
{
	GENERATED_BODY()
	
    float m_timer;
  float m_totalDistance;
  float m_currentDistance;
  FVector m_rightVector;

  enum state{
    INITIAL_DELAY,
    TO_RIGHT,
    TO_LEFT,
    RIGHT_DELAY,
    LEFT_DELAY,
  }m_state;

  void doMovement(float DeltaSeconds);
	
public:
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

};
