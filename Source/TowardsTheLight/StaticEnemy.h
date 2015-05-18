// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "StaticEnemy.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AStaticEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStaticEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
  virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

  UPROPERTY()
    USceneComponent* OurVisibleComponent;
	
};
