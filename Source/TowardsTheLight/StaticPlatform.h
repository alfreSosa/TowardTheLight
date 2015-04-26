// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StaticPlatform.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AStaticPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticPlatform();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
  void ReceiveActorBeginOverlap(AActor * OtherActor) override;

  UPROPERTY()
    USceneComponent* OurVisibleComponent;
	
	
};
