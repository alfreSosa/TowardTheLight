// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

  AMyGameMode(const FObjectInitializer& ObjectInitializer);  // Our added constructor
  UFUNCTION()
    void AddPoints(float points);
  UFUNCTION()
    void OrbPicked();

private:
  float m_actualPoints;
  float m_countOrbs;
};

