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

  class AOviCharacter *m_player;
public:
  // Called when the game starts or when spawned
  //virtual void BeginPlay() override;
	
  // Called every frame
  //virtual void Tick(float DeltaSeconds) override { }
	
};
