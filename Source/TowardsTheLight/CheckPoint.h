// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

class ATowardsTheLightGameMode;
UCLASS()
class TOWARDSTHELIGHT_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
  //properties
  UPROPERTY()
    UBoxComponent *Trigger;
  UPROPERTY(EditAnywhere)
    UStaticMeshComponent* CheckPointVisibleComponent;
  //functions
  ACheckPoint();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
  void RegisterDelegate();
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  void RestoreInitialState();
  //Delegates
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
  //properties
  ATowardsTheLightGameMode *m_gameMode;
  bool m_loaded;
  bool m_enter;
	
	
};
