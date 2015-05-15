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
  
  float m_rightPosition;
  float m_leftPosition;
  float m_initialTimer;
  float m_direction;

public:
  AMobilePlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float RightDistance;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float LeftDistance;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float RightDelay;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float LeftDelay;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float Speed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MobilePlatform)
    float InitialDelay;
};
