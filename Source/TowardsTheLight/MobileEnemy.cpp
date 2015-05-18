// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MobileEnemy.h"


AMobileEnemy::AMobilePlatform() {
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
}


void AMobileEnemy::BeginPlay() {
  Super::BeginPlay();

  m_totalDistance = RightDistance;
}

void AMobileEnemy::Tick(float DeltaSeconds) {
  Super::BeginPlay();

  doMovement(DeltaSeconds);
}

void AMobileEnemy::doMovement(float DeltaSeconds){
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
      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += dist * m_rightVector;
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
      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += dist * -m_rightVector;
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


