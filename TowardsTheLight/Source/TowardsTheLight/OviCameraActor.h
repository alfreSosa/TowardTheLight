// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include "OviCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AOviCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
  // Sets default values for this character's properties
  AOviCameraActor();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
	
};
