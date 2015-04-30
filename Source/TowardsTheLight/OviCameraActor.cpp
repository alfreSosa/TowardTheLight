// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviCameraActor.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"

//#include "Kismet/KismetMathLibrary.h"

AOviCameraActor::AOviCameraActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  //setting
  Padding = 100;
  DistanceCameraToPlayer = 500;
  CameraSpeed = 50;
  m_sqrt2 = FMath::Sqrt(2);
  m_sqrt3 = FMath::Sqrt(3);
}

void AOviCameraActor::BeginPlay(){
  Super::BeginPlay();
  
  GetAutoActivatePlayerIndex();
  m_player = NULL;
  for (TActorIterator< APawn > ActorItr(GetWorld()); ActorItr; ++ActorItr) {
    if (ActorItr->ActorHasTag("Player")){
		  m_player = (APlayerOvi*)*ActorItr;
      break;
    }
  }

  float limit = FVector::DotProduct(m_player->GetActorLocation(), m_player->GetActorForwardVector());
  m_limitPadding = abs(limit) - Padding;
  m_cameraDistance = abs(limit) + DistanceCameraToPlayer;

  AOviPlayerController* oviPlayerController = (AOviPlayerController* )GetWorld()->GetFirstPlayerController();
  if (oviPlayerController)
    oviPlayerController->SetViewTarget(this);
}


void AOviCameraActor::Tick(float DeltaSeconds){
  SetPosition(DeltaSeconds);
  SetOrientation();
}

void AOviCameraActor::SetPosition(float DeltaSeconds){
  FVector finalPos = m_player->GetActorLocation();
  if (finalPos.X > m_limitPadding)
    finalPos.X += DistanceCameraToPlayer;
  else if (finalPos.X < -m_limitPadding)
    finalPos.X += -DistanceCameraToPlayer;

  if (finalPos.Y > m_limitPadding)
    finalPos.Y += DistanceCameraToPlayer;
  else if (finalPos.Y < -m_limitPadding)
    finalPos.Y += -DistanceCameraToPlayer;

  if (finalPos.Z > m_limitPadding)
    finalPos.Z += DistanceCameraToPlayer;
  else if (finalPos.Z < -m_limitPadding)
    finalPos.Z += -DistanceCameraToPlayer;

  FVector absFinalPos(finalPos.GetAbs());

  if (absFinalPos.X >= m_cameraDistance && absFinalPos.Y >= m_cameraDistance && absFinalPos.Z >= m_cameraDistance) {
    finalPos = finalPos / m_sqrt3;
  }
  else if (absFinalPos.X >= m_cameraDistance && absFinalPos.Y >= m_cameraDistance) {
    finalPos.X = finalPos.X / m_sqrt2;
    finalPos.Y = finalPos.Y / m_sqrt2;
  }
  else if (absFinalPos.X >= m_cameraDistance && absFinalPos.Z >= m_cameraDistance) {
    finalPos.X = finalPos.X / m_sqrt2;
    finalPos.Z = finalPos.Z / m_sqrt2;
  }
  else if (absFinalPos.Y >= m_cameraDistance && absFinalPos.Z >= m_cameraDistance) {
    finalPos.Y = finalPos.Y / m_sqrt2;
    finalPos.Z = finalPos.Z / m_sqrt2;
  }

  FVector currentPos = this->GetActorLocation();

  SetActorLocation(FMath::Lerp(finalPos, currentPos, CameraSpeed * DeltaSeconds));
}

void AOviCameraActor::SetOrientation(){
  FVector forward = m_player->GetActorLocation() - this->GetActorLocation();
  forward.Normalize();
  FVector side = FVector::CrossProduct(m_player->GetActorUpVector(), forward);
  side.Normalize();

  FRotator rot = FRotationMatrix::MakeFromXY(forward, side).Rotator();
  this->SetActorRotation(rot);

  //FVector side = FVector::CrossProduct(forward, m_player->GetActorUpVector());
  //side.Normalize();
  //FVector up = FVector::CrossProduct(side, forward);
  //FRotator rot = FRotationMatrix::MakeFromXZ(forward, up).Rotator();
  //this->SetActorRotation(rot);
}