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
public:
  //properties
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    int32 NumberOfIntermitences;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    float InitialTimeDelay;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    float EndTimeDelay;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    float TimeInStateVisible;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    float TimeInStateNoVisible;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    bool StartVisible;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatform)
    bool RestartAtEnd;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    bool Enabled;
  //functions
  AIntermittentPlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
  void ChangeEnabled(bool enabled);
private:
  //functions
  void runStateMachine(float DeltaSeconds);
  //properties
  enum State {INITIALDELAY, ON, OFF, ENDDELAY};
  State m_actualState;
  float m_elapsedTime;
  bool m_countIntermittences;
  int32 m_counterIntermittences;
	
};
