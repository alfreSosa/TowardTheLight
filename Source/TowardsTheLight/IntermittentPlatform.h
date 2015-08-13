// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticPlatform.h"
#include "IntermittentPlatform.generated.h"

/**
 * 
 */
class AIntermittentManager;
UCLASS()
class TOWARDSTHELIGHT_API AIntermittentPlatform : public AStaticPlatform
{
  GENERATED_BODY()
  //material
  UMaterialInstanceDynamic *IntermittentPlatformMaterial;

public:
  //properties
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformTiming)
    int32 NumberOfIntermitences;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformTiming)
    float InitialTimeDelay;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformTiming)
    float EndTimeDelay;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformTiming)
    float TimeInStateVisible;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformTiming)
    float TimeInStateNoVisible;

  UPROPERTY(EditAnywhere, Category = IntermittentPlatformState)
    bool StartVisible;
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformState)
    bool Loop;

  //functions
  AIntermittentPlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
  virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
  virtual void ReceiveActorEndOverlap(AActor* OtherActor) override;
  void InitOwner(AIntermittentManager *owner);
  void Init();
  void RestoreInitialState();
private:
  //functions
  void runStateMachine(float DeltaSeconds);
  //manager
  AIntermittentManager *m_owner;
  //properties
  enum State {INITIALDELAY, ON, OFF, ENDDELAY};
  State m_actualState;
  float m_elapsedTime;
  bool m_countIntermittences;
  int32 m_counterIntermittences;
  bool m_playerIsTouching;
  bool m_isVisible;
  bool Enabled;
  bool m_finished;
	
};
