// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "OviCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AOviCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
  AOviCameraManager(const FObjectInitializer& ObjectInitializer);  // Our added constructor

public:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
};
