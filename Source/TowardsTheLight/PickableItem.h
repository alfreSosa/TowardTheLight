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
	
  virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

  UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickableItem)
    float Points;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PickableItem)
    bool IsOrb;
private:
  const float DEFAULT_POINTS = 100.0f;
	
  bool m_collected;
};
