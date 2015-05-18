// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tower.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API ATower : public APawn
{
	GENERATED_BODY()

  UPROPERTY()
    UBoxComponent *Trigger;
public:	
	// Sets default values for this actor's properties
	ATower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
  UPROPERTY()
    UStaticMeshComponent* Body;

  UPROPERTY()
    UStaticMeshComponent* Entrance;
	
  void RegisterDelegate();
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
