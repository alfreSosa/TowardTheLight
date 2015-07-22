// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "IntermittentPlatform.h"
#include "TimeManager.h"

AIntermittentPlatform::AIntermittentPlatform() {
  PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);

  RootComponent->SetMobility(EComponentMobility::Movable);
  OurVisibleComponent->SetMobility(EComponentMobility::Movable);

  //Init default properties
  //public
  NumberOfIntermitences = 0;
  TimeInState = 1.0f;
  Enabled = true;
  //private
  m_elapsedTime = 0.0f;
  m_actualState = State::ON;
}

void AIntermittentPlatform::BeginPlay() {

}


void AIntermittentPlatform::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  m_elapsedTime += DeltaSeconds;
  switch (m_actualState)
  {
  case AIntermittentPlatform::ON:
    if (m_elapsedTime >= TimeInState) {
      m_actualState = State::OFF; 
      this->SetActorHiddenInGame(true);
      this->Tags.Remove("Platform");
      m_elapsedTime = 0.0f;
    }
    break;
  case AIntermittentPlatform::OFF:
    if (m_elapsedTime >= TimeInState) {
      m_actualState = State::ON;
      this->SetActorHiddenInGame(false);
      this->Tags.Add("Platform");
      m_elapsedTime = 0.0f;
    }
    break;
  default:
    break;
  }
}
