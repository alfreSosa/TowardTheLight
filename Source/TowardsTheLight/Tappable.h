// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tappable.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API ATappable : public AActor
{
	GENERATED_BODY()
	
public:	
  //Properties
  UPROPERTY(EditAnywhere, Category = Activator)
    USceneComponent* MeshActivator;
  UPROPERTY()
    UBoxComponent *Trigger;
	// Sets default values for this actor's properties
	ATappable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
  virtual void ReceiveActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex) override;
  //virtual void OnActorBeginOverlap() override;
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  //Custom public functions
  void RegisterDelegate();
  virtual void Activate(bool enabled);
  virtual void Execute();
  //Delegates
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
  //MechanismState
  bool m_isPlayerOn;
	
};
