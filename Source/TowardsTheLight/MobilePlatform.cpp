// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MobilePlatform.h"


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

}

void AMobilePlatform::BeginPlay() {
  Super::BeginPlay();
  this->Tags.Add("MobilePlatform");
  m_totalDistance = RightDistance;
}

void AMobilePlatform::Tick(float DeltaSeconds) {
  Super::BeginPlay();
  doMovement(DeltaSeconds);
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

//ESTO YA NO HARIA FALTA(CREO)
//void AMobilePlatform::ReceiveActorBeginOverlap(AActor* OtherActor) {
//  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("collision!!!!!")));
//  if (OtherActor->ActorHasTag("Player")) {
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("player!!!!!")));
//    //this->SetActorEnableCollision(false);
//    //PrimaryActorTick.bCanEverTick = false;
//    //SetActorLocation(FVector(0, 0, 0));
//    //AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
//    //if (gameMode) {
//    //  gameMode->AddPoints(this->Points);
//    //  if (IsOrb)
//    //    gameMode->OrbPicked();
//
//    //}
//  }
//}

FVector AMobilePlatform::GetPlatformMovement() const{
  return m_movement;
}