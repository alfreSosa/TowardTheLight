// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticPlatform.h"
#include "IntermittentPlatform.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AIntermittentPlatform : public AStaticPlatform
{
  GENERATED_BODY()
  AIntermittentPlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
//properties
public:
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    int32 NumberOfIntermitences;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    float TimeInState;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    bool Enabled;
private:
  enum State {ON, OFF};
  State m_actualState;
  float m_elapsedTime;
	
};
