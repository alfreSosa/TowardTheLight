// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StaticPlatform.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AStaticPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaticPlatform();

  UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;
	
	
};
