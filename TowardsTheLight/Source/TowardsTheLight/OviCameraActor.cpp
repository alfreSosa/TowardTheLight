// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviCameraActor.h"

AOviCameraActor::AOviCameraActor(){
  Auto
}

void AOviCameraActor::BeginPlay(){
  Super::BeginPlay();
  
  //AutoPossessPlayer = EAutoReceiveInput::Player0;
}
