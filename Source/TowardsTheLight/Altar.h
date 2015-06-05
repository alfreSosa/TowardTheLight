// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Altar.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AAltar : public AActor
{
	GENERATED_BODY()
public:	
	AAltar();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  UPROPERTY(EditAnywhere, Category = Altar)
    bool GiveKey;
  UPROPERTY(EditAnywhere, Category = Altar)
    FLinearColor ColorKey;
  UPROPERTY(EditAnywhere, Category = Altar)
    USceneComponent* OurVisibleComponent;
  UPROPERTY(EditAnywhere, Category = Altar)
    UStaticMeshComponent* Orientation;

private:
  UPROPERTY()
    UBoxComponent *Trigger;
  void RegisterDelegate();
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	
};