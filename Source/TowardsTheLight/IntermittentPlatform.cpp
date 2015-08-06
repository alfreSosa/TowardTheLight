// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "IntermittentPlatform.h"
#include "IntermittentManager.h"

AIntermittentPlatform::AIntermittentPlatform() {
  //PrimaryActorTick.bCanEverTick = true; //acordarse de desactivar esto con el manager
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
  m_owner = nullptr;
}

void AIntermittentPlatform::BeginPlay() {
  this->Tags.Add("IntermittentPlatform");
  Init();
}


void AIntermittentPlatform::Init() {

  if (this->ActorHasTag("Platform"))
    this->Tags.Remove("Platform");
  m_actualState = State::INITIALDELAY;

  if (StartVisible) {
    this->SetActorHiddenInGame(false);
    this->Tags.Add("Platform");
  }
  else {
    this->SetActorHiddenInGame(true);
    this->Tags.Remove("Platform");
  }

  m_isVisible = StartVisible;
  m_playerIsTouching = false;
  m_counterIntermittences = NumberOfIntermitences;
  m_countIntermittences = (NumberOfIntermitences == 0) ? false : true;
  m_elapsedTime = 0.0f;
}

void AIntermittentPlatform::Tick(float DeltaSeconds) {
  if(m_countIntermittences && m_counterIntermittences <= 0)
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

        m_isVisible = false;
        if (m_playerIsTouching)
          m_owner->AlertBlocking(true);
       
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

        m_isVisible = true;
        m_actualState = State::ON;
        this->SetActorHiddenInGame(false);
        this->Tags.Add("Platform");
        m_elapsedTime = 0.0f;
      }
      break;
    case AIntermittentPlatform::ENDDELAY:
      if (m_elapsedTime >= EndTimeDelay) {
        //Enabled = Loop;
        m_elapsedTime = 0.0f;
        m_actualState = (Loop) ? INITIALDELAY : ENDDELAY;
        if (!Loop)
          m_owner->AlertFinish();
        //PROBANDO EL RESET
        m_isVisible = StartVisible;
        m_counterIntermittences = (Loop) ? NumberOfIntermitences : 0;        
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
    if (!m_isVisible) {
      m_owner->AlertBlocking(true);
    }
  }
}

void AIntermittentPlatform::ReceiveActorEndOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player")) {
    m_playerIsTouching = false;
    m_owner->AlertBlocking(false);
  }

}

void AIntermittentPlatform::InitOwner(AIntermittentManager *owner) {
  m_owner = owner;
}




