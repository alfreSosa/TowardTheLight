// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MobilePlatform.h"


AMobilePlatform::AMobilePlatform() {
  PrimaryActorTick.bCanEverTick = true;

  //setting
  RightDistance = 0.f;
  LeftDistance = 0.f;
  RightDelay = 1.f;
  LeftDelay = 1.f;
  Speed = 100.f;
  InitialDelay = 0.f;

  m_initialTimer = 0;
  m_direction = 1;
}

void AMobilePlatform::BeginPlay() {
  Super::BeginPlay();

  m_rightPosition = FVector::DotProduct(FVector(1, 1, 1), GetActorRightVector()) + RightDistance;
  m_leftPosition = FVector::DotProduct(FVector(1, 1, 1), GetActorRightVector()) - LeftDistance;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("dddddds: x: %f, y: %f"), m_rightPosition, m_leftPosition));

}

void AMobilePlatform::Tick(float DeltaSeconds) {
  Super::BeginPlay();
  if (m_initialTimer < InitialDelay)
    m_initialTimer += DeltaSeconds;
  else{
    FVector right = GetActorRightVector();
    FVector location = GetActorLocation();
    location += Speed * m_direction * DeltaSeconds * right;


    float pos = FVector::DotProduct(FVector(1, 1, 1), GetActorRightVector());
    if (pos > m_rightPosition || pos < m_leftPosition)
      m_direction *= -1;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f, z: %f "), location.X, location.Y, location.Z));


    SetActorLocation(location);
  }
}
