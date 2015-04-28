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
  CameraDistance = 1500;
  m_sqrt2 = FMath::Sqrt(2);
  m_sqrt3 = FMath::Sqrt(3);
}

void AOviCameraActor::BeginPlay(){
  Super::BeginPlay();
  
  //AutoPossessPlayer = EAutoReceiveInput::Player0;

  GetAutoActivatePlayerIndex();
  m_player = NULL;
  for (TActorIterator< APawn > ActorItr(GetWorld()); ActorItr; ++ActorItr) {
    if (ActorItr->ActorHasTag("Player")){
		m_player = (APlayerOvi*)*ActorItr;
      break;
    }
  }

  m_limit = FVector::DotProduct(m_player->GetActorLocation(), m_player->GetActorForwardVector());
  m_limit = abs(m_limit) - Padding;



  AOviPlayerController* oviPlayerController = (AOviPlayerController* )GetWorld()->GetFirstPlayerController();
  if (oviPlayerController)
    oviPlayerController->SetViewTarget(this);
}


void AOviCameraActor::Tick(float DeltaSeconds){
  SetPosition();
  SetOrientation();
}

void AOviCameraActor::SetPosition(){
  FVector pos = m_player->GetActorLocation();
  if (pos.X > m_limit)
    pos.X = CameraDistance;
  else if (pos.X < -m_limit)
    pos.X = -CameraDistance;

  if (pos.Y > m_limit)
    pos.Y = CameraDistance;
  else if (pos.Y < -m_limit)
    pos.Y = -CameraDistance;

  if (pos.Z > m_limit)
    pos.Z = CameraDistance;
  else if (pos.Z < -m_limit)
    pos.Z = -CameraDistance;

  FVector absPos(pos.GetAbs());

  if (absPos.X == CameraDistance && absPos.Y == CameraDistance && absPos.Z == CameraDistance) {
    pos = pos / m_sqrt3;
    //m_lerp = true;
  }
  else if (absPos.X == CameraDistance && absPos.Y == CameraDistance) {
    pos.X = pos.X / m_sqrt2;
    pos.Y = pos.Y / m_sqrt2;
    //m_lerp = true;
  }
  else if (absPos.X == CameraDistance && absPos.Z == CameraDistance) {
    pos.X = pos.X / m_sqrt2;
    pos.Z = pos.Z / m_sqrt2;
    //m_lerp = true;
  }
  else if (absPos.Y == CameraDistance && absPos.Z == CameraDistance) {
    pos.Y = pos.Y / m_sqrt2;
    pos.Z = pos.Z / m_sqrt2;
    //m_lerp = true;
  }
  //else {
  //  m_lerp = false;
  //}

  SetActorLocation(pos);
}

void AOviCameraActor::SetOrientation(){
  FVector dir = m_player->GetActorLocation() - GetActorLocation();
  dir.Normalize();
  FRotator newRot = FRotationMatrix::MakeFromX(dir).Rotator();
  
  //FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), m_player->GetActorLocation());

  
  //revisar para cuando cambie a las caras superior e inferior

  SetActorRotation(newRot);
}