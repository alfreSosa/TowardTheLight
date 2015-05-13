// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
  virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

  UPROPERTY()
    USceneComponent* OurVisibleComponent;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InputControl)
    float Points;
private:
  const float DEFAULT_POINTS = 100.0f;
	
};
