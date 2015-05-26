// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticPlatform.h"
#include "MobilePlatform.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AMobilePlatform : public AStaticPlatform
{
  GENERATED_BODY()
  
  class APlayerOvi *m_player;
  float m_timer;
  float m_totalDistance;
  float m_currentDistance;
  FVector m_rightVector;
  FVector m_movement;

  enum state{
    INITIAL_DELAY,
    TO_RIGHT,
    TO_LEFT,
    RIGHT_DELAY,
    LEFT_DELAY,
  }m_state;

  void doMovement(float DeltaSeconds);
public:
  AMobilePlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;

  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float RightDistance;
  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float LeftDistance;
  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float RightDelay;
  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float LeftDelay;
  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float Speed;
  UPROPERTY(EditAnywhere, Category = MobilePlatform)
    float InitialDelay;

  //FVector GetPlatformMovement() const;
};
