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
  Loop = StartVisible = Enabled = true;
  //private
  m_countIntermittences = true;
  m_counterIntermittences = NumberOfIntermitences;
  m_elapsedTime = 0.0f;
  m_actualState = State::INITIALDELAY;
  m_playerIsTouching = false;
  Previous = nullptr;
  Next = nullptr;
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
  m_isVisible = StartVisible;
  m_playerIsTouching = false;
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
      if (!StartVisible && m_playerIsTouching)
        m_elapsedTime -= DeltaSeconds;

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
        m_isVisible = false;
        m_actualState = State::OFF;
        this->SetActorHiddenInGame(true);
        this->Tags.Remove("Platform");
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::OFF:
      if (m_playerIsTouching)
        m_elapsedTime -= DeltaSeconds;
      if (m_elapsedTime >= TimeInStateNoVisible) {
        if (m_countIntermittences)
          m_counterIntermittences--;

        m_isVisible = true;
        m_actualState = State::ON;
        this->SetActorHiddenInGame(false);
        this->Tags.Add("Platform");
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::ENDDELAY:
      if (m_playerIsTouching && !m_isVisible)
        m_elapsedTime -= DeltaSeconds;
      if (m_elapsedTime >= EndTimeDelay) {
        Enabled = Loop;
        m_elapsedTime = 0.0f;
        m_actualState = (Loop) ? INITIALDELAY : ENDDELAY;
        if (Loop) {
          m_isVisible = StartVisible;
          m_counterIntermittences = (Loop) ? NumberOfIntermitences : 0;
          if (StartVisible) {
            this->SetActorHiddenInGame(false);
          }
          else {
            this->SetActorHiddenInGame(true);
            this->Tags.Remove("Platform");
          }
        }
      }
      break;
    default:
      break;
    }
  }
}

void AIntermittentPlatform::ReceiveActorBeginOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player")) {
    m_playerIsTouching = true;
    AlertPlayerTouching(true, this);
  }
}

void AIntermittentPlatform::ReceiveActorEndOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player")) {
    m_playerIsTouching = false;
    AlertPlayerTouching(false, this);
  }

}


void AIntermittentPlatform::ChangeEnabled(bool enabled) {
  Enabled = enabled;
  //se restaura las intermitencias
  m_actualState = INITIALDELAY;
  m_counterIntermittences = NumberOfIntermitences;
}

void AIntermittentPlatform::AlertPlayerTouching(bool player, AIntermittentPlatform *platform) {
  //esto no funciona, tengo que plantearlo bien toda la estructura.
  /*m_playerIsTouching = player;
  
  if (platform != this && m_playerIsTouching)
    Enabled = false;
  else if (platform != this && !m_playerIsTouching)
    Enabled = true;

  if (Next && Next != platform) 
    Next->AlertPlayerTouching(player, this);
  if (Previous && Previous != platform) 
    Previous->AlertPlayerTouching(player, this);*/
}


