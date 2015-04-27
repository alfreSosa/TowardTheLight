// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviPlayerController.h"
#include "OviCameraManager.h"


AOviPlayerController::AOviPlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
  PlayerCameraManagerClass = AOviCameraManager::StaticClass();
  //UTouchInterface *f =  CurrentTouchInterface;
}
