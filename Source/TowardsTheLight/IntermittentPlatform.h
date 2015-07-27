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
  UPROPERTY(EditAnywhere, Category = IntermittentPlatformState)
    bool Enabled;

  UPROPERTY(EditAnywhere, Category = PlatformSynchronize)
    AIntermittentPlatform *Previous;
  UPROPERTY(EditAnywhere, Category = PlatformSynchronize)
    AIntermittentPlatform *Next;

  //functions
  AIntermittentPlatform();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
  virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
  virtual void ReceiveActorEndOverlap(AActor* OtherActor) override;
  void ChangeEnabled(bool enabled);
  bool isEnabled();
  void InitByMechanism(bool disableAtEnd, int32 numActions);
  void AlertPlayerTouching(bool player, AIntermittentPlatform *platform);
private:
  //functions
  void runStateMachine(float DeltaSeconds);
  //properties
  enum State {INITIALDELAY, ON, OFF, ENDDELAY};
  State m_actualState;
  float m_elapsedTime;
  bool m_countIntermittences;
  int32 m_counterIntermittences;
  bool m_playerIsTouching;
  bool m_disableTimer;
  bool m_isVisible;
  bool m_restart;
	
};
