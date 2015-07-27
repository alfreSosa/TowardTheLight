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
  m_disableTimer = false;
  m_restart = false;
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
  m_restart = false;
}


void AIntermittentPlatform::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  if (m_restart) {
    m_actualState = INITIALDELAY;
    m_restart = false;
    m_counterIntermittences = NumberOfIntermitences;
  }
 
  if(m_countIntermittences && m_counterIntermittences <= 0)
    m_actualState = ENDDELAY;

  runStateMachine(DeltaSeconds);
}

void AIntermittentPlatform::runStateMachine(float DeltaSeconds) {
  if (Enabled) {
    if (!m_disableTimer)
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
        if (m_playerIsTouching) {
          m_disableTimer = true;
          AlertPlayerTouching(true, this);
        }
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
        Enabled = Loop;
        m_elapsedTime = 0.0f;
        m_actualState = (Loop && !m_restart) ? INITIALDELAY : ENDDELAY;
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
      m_disableTimer = true;
      AlertPlayerTouching(true, this);
    }
  }
}

void AIntermittentPlatform::ReceiveActorEndOverlap(AActor* OtherActor) {
  //nota para el futuro: puede darse que se procese primero todos los beginoverlap y luego los end, si estamos dentro de una no visible y pasamos a otra no visible es posible que le meta en una plataforma sino se respetan las distancias.
  if (OtherActor->ActorHasTag("Player")) {
    m_playerIsTouching = false;
    m_disableTimer = false;
    AlertPlayerTouching(false, this);
  }

}

void AIntermittentPlatform::ChangeEnabled(bool enabled) {
  Enabled = enabled;
  if (Enabled && m_actualState == ENDDELAY)
    m_restart = true;
}

void AIntermittentPlatform::AlertPlayerTouching(bool player, AIntermittentPlatform *platform) {
  m_disableTimer = player;
  //para evitar bucles infinitos de llamada los if
  if (Next && Next != platform) 
    Next->AlertPlayerTouching(player, this);
  if (Previous && Previous != platform) 
    Previous->AlertPlayerTouching(player, this);
}

bool AIntermittentPlatform::isEnabled() {
  return Enabled;
}

void AIntermittentPlatform::InitByMechanism(bool disableAtEnd, int32 numActions) {
  Loop = !disableAtEnd;
  m_counterIntermittences = NumberOfIntermitences;
  m_countIntermittences = (NumberOfIntermitences == 0) ? false : true;
}


