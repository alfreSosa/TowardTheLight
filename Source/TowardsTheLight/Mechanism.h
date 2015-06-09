// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Mechanism.generated.h"

class AMobilePlatform;

UCLASS()
class TOWARDSTHELIGHT_API AMechanism : public AActor
{
	GENERATED_BODY()
	
public:
  //Properties
  UPROPERTY(EditAnywhere, Category = Activator)
    USceneComponent* MeshActivator;
  UPROPERTY()
    UBoxComponent *Trigger;
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    AActor *Target;
	//constructor
	AMechanism();

	//Class Functions
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  virtual void ReceiveActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex) override;
  //virtual void OnActorBeginOverlap() override;
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  //Custom public functions
  void RegisterDelegate();

  //Delegates
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
  //MechanismState
  bool m_isEnabled;
  bool m_isPlayerOn;
  //TargetVariables
  AMobilePlatform *m_mobileTarget;
	
};
