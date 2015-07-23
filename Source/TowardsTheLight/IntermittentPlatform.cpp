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
  InitialTimeDelay = TimeInStateNoVisible = TimeInStateVisible = 1.0f;
  EndTimeDelay = 0.0f;
  StartVisible = Enabled = true;
  //private
  m_countIntermittences = true;
  m_counterIntermittences = NumberOfIntermitences;
  m_elapsedTime = 0.0f;
  m_actualState = State::INITIALDELAY;
}

void AIntermittentPlatform::BeginPlay() {
  this->Tags.Add("IntermittentPlatform");
  m_actualState = State::INITIALDELAY;
  if (StartVisible) {
    this->SetActorHiddenInGame(false);
  }
  else {
    this->SetActorHiddenInGame(true);
    this->Tags.Remove("Platform");
  }
  m_counterIntermittences = NumberOfIntermitences;
  m_countIntermittences = (NumberOfIntermitences == 0) ? false : true;
}


void AIntermittentPlatform::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  if (m_countIntermittences && m_counterIntermittences <= 0)
    m_actualState = ENDDELAY;

  runStateMachine(DeltaSeconds);
}

void AIntermittentPlatform::runStateMachine(float DeltaSeconds) {
  if (Enabled) {
    m_elapsedTime += DeltaSeconds;
    switch (m_actualState)
    {
    case INITIALDELAY:
      if (m_elapsedTime >= InitialTimeDelay) {
        if (StartVisible)
          m_actualState = State::ON;
        else
          m_actualState = State::OFF;
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::ON:
      if (m_elapsedTime >= TimeInStateVisible) {
        if (m_countIntermittences)
          m_counterIntermittences--;

        m_actualState = State::OFF;
        this->SetActorHiddenInGame(true);
        this->Tags.Remove("Platform");
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::OFF:
      if (m_elapsedTime >= TimeInStateNoVisible) {
        if (m_countIntermittences)
          m_counterIntermittences--;

        m_actualState = State::ON;
        this->SetActorHiddenInGame(false);
        this->Tags.Add("Platform");
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::ENDDELAY:
      if (m_elapsedTime >= EndTimeDelay) {
        Enabled = RestartAtEnd;
        m_elapsedTime = 0.0f;
        m_actualState = (RestartAtEnd) ? INITIALDELAY : ENDDELAY;
      }
      break;
    default:
      break;
    }
  }
}

void AIntermittentPlatform::ChangeEnabled(bool enabled) {
  Enabled = enabled;
  //se restaura las intermitencias
  m_actualState = INITIALDELAY;
  m_counterIntermittences = NumberOfIntermitences;
}


