// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();
  
}




