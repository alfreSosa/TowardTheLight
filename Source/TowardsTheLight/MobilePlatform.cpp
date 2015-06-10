// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MobilePlatform.h"
#include "PlayerOvi.h"
#include "TimeManager.h"


AMobilePlatform::AMobilePlatform() {
  PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);

  //setting
  RightDistance = 100.f;
  LeftDistance = 100.f;
  RightDelay = 1.f;
  LeftDelay = 1.f;
  Speed = 100.f;
  InitialDelay = 1.f;

  //private variables
  m_timer = 0;
  m_state = INITIAL_DELAY;
  m_currentDistance = 0;
  m_isPlayerOn = false;
}

void AMobilePlatform::BeginPlay() {
  Super::BeginPlay();
  this->Tags.Add("MobilePlatform");
  m_totalDistance = RightDistance;
}

void AMobilePlatform::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  if (Enabled) {
    if (!m_player)
      for (TActorIterator< APawn > ActorItr(GetWorld()); ActorItr; ++ActorItr)
        if (ActorItr->ActorHasTag("Player")){
          m_player = (APlayerOvi*)*ActorItr;
          break;
        }

    doMovement(DeltaSeconds);
    if (m_player && m_isPlayerOn)
      m_player->OnMobilePlatform(this, m_movement);
  }
}

void AMobilePlatform::doMovement(float DeltaSeconds){
  m_movement = FVector(0);
  switch (m_state){
  case INITIAL_DELAY:
    if (m_timer < InitialDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_RIGHT;
      m_rightVector = GetActorRightVector();
    }
    break;
  case TO_RIGHT:{
    float dist = Speed * DeltaSeconds;
    if (m_totalDistance - m_currentDistance < dist)
      dist = m_totalDistance - m_currentDistance;

    if (m_currentDistance < m_totalDistance){
      m_movement = dist * m_rightVector;

      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += m_movement;
      SetActorLocation(location);
    }
    else{
      m_state = RIGHT_DELAY;
      m_currentDistance = 0;
      if (m_totalDistance == RightDistance)
        m_totalDistance += LeftDistance;
    }
  }
    break;
  case TO_LEFT:{
    float dist = Speed * DeltaSeconds;
    if (m_totalDistance - m_currentDistance < dist)
      dist = m_totalDistance - m_currentDistance;

    if (m_currentDistance < m_totalDistance){
      m_movement = dist * -m_rightVector;

      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += m_movement;
      SetActorLocation(location);
    }
    else{
      m_state = LEFT_DELAY;
      m_currentDistance = 0;
    }
  }
    break;
  case RIGHT_DELAY:
    if (m_timer < RightDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_LEFT;
    }
    break;
  case LEFT_DELAY:
    if (m_timer < LeftDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_RIGHT;
    }
    break;
  }
}

void AMobilePlatform::SetPlayerOn(bool on) {
  m_isPlayerOn = on;
}

void AMobilePlatform::ChangeEnabled(bool enabled) {
  Enabled = enabled;
}

bool AMobilePlatform::isEnabled() {
  return Enabled;
}
//FVector AMobilePlatform::GetPlatformMovement() const{
//  return m_movement;
//}