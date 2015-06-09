// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Mechanism.generated.h"

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

	//constructor
	AMechanism();

	//Class Functions
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  virtual void ReceiveActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex) override;
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  //Custom public functions
  void RegisterDelegate();

  //Delegates
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
  bool m_isPlayerOn;
	
};
