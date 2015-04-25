// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviCameraManager.h"
#include "OviCameraActor.h"
#include "OviPlayerController.h"


AOviCameraManager::AOviCameraManager(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
  AnimCameraActor; //creo que hay que crear la camara y asignarla a esta variable y faltaria ver como indicarle el player al que sigue
  PCOwner;
 // InitializeFor();
}

void AOviCameraManager::BeginPlay(){
  this->PCOwner = this->GetWorld()->GetFirstPlayerController();

}

