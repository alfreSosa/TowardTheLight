// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviPlayerController.h"


AOviPlayerController::AOviPlayerController(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
  //UTouchInterface *f =  CurrentTouchInterface;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
}



