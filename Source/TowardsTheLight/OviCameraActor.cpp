// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviCameraActor.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"

AOviCameraActor::AOviCameraActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  //setting
  Padding = 200;
  DistanceCameraToPlayer = 500;
  //CameraSpeed = 50;
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

  m_limitWorld = FVector::DotProduct(m_player->GetActorLocation(), m_player->GetActorForwardVector());
  m_limitPadding = abs(m_limitWorld) - Padding;
  m_cameraDistance = abs(m_limitWorld) + DistanceCameraToPlayer;
  m_ruleOfThree = (m_cameraDistance - m_limitPadding) / (m_limitWorld - m_limitPadding);

  //if (CameraSpeed < 0)
  //  CameraSpeed = 0;
  //if (CameraSpeed > MAX_CAMERA_SPEED)
  //  CameraSpeed = MAX_CAMERA_SPEED;

  //m_deltaLerp = CameraSpeed / MAX_CAMERA_SPEED;

  AOviPlayerController* oviPlayerController = (AOviPlayerController* )GetWorld()->GetFirstPlayerController();
  if (oviPlayerController)
    oviPlayerController->SetViewTarget(this);
}


void AOviCameraActor::Tick(float DeltaSeconds){
  SetPosition();
  SetOrientation();
}

void AOviCameraActor::SetPosition(){
  // TRY LERP
  //FVector finalPos = m_player->GetActorLocation();
  //if (finalPos.X > m_limitPadding)
  //  finalPos.X = m_cameraDistance;
  //else if (finalPos.X < -m_limitPadding)
  //  finalPos.X = -m_cameraDistance;

  //if (finalPos.Y > m_limitPadding)
  //  finalPos.Y = m_cameraDistance;
  //else if (finalPos.Y < -m_limitPadding)
  //  finalPos.Y = -m_cameraDistance;

  //if (finalPos.Z > m_limitPadding)
  //  finalPos.Z = m_cameraDistance;
  //else if (finalPos.Z < -m_limitPadding)
  //  finalPos.Z = -m_cameraDistance;

  //FVector absFinalPos(finalPos.GetAbs());

  //if (absFinalPos.X == m_cameraDistance && absFinalPos.Y == m_cameraDistance && absFinalPos.Z == m_cameraDistance) {
  //  finalPos = finalPos / 1.2;
  //}
  //else if (absFinalPos.X == m_cameraDistance && absFinalPos.Y == m_cameraDistance) {
  //  finalPos.X = finalPos.X / 1.1;
  //  finalPos.Y = finalPos.Y / 1.1;
  //}
  //else if (absFinalPos.X == m_cameraDistance && absFinalPos.Z == m_cameraDistance) {
  //  finalPos.X = finalPos.X / 1.1;
  //  finalPos.Z = finalPos.Z / 1.1;
  //}
  //else if (absFinalPos.Y == m_cameraDistance && absFinalPos.Z == m_cameraDistance) {
  //  finalPos.Y = finalPos.Y / 1.1;
  //  finalPos.Z = finalPos.Z / 1.1;
  //}

  //FVector currentPos = this->GetActorLocation();

  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("delta: %f "), m_deltaLerp));

  //SetActorLocation(FMath::Lerp(currentPos, finalPos, m_deltaLerp));


  // TRY RULE OF THREE
  FVector finalPos = m_player->GetActorLocation();
  FVector dist(0, 0, 0);
  if (finalPos.X > m_limitPadding)
    dist.X = finalPos.X - m_limitPadding;
  else if (finalPos.X < -m_limitPadding)
    dist.X = finalPos.X + m_limitPadding;

  if (finalPos.Y > m_limitPadding)
    dist.Y = finalPos.Y - m_limitPadding;
  else if (finalPos.Y < -m_limitPadding)
    dist.Y = finalPos.Y + m_limitPadding;

  if (finalPos.Z > m_limitPadding)
    dist.Z = finalPos.Z - m_limitPadding;
  else if (finalPos.Z < -m_limitPadding)
    dist.Z = finalPos.Z + m_limitPadding;

  finalPos += dist * m_ruleOfThree;

  SetActorLocation(finalPos);


}

void AOviCameraActor::SetOrientation(){
  FVector forward = m_player->GetActorLocation() - this->GetActorLocation();
  forward.Normalize();
  FVector side = FVector::CrossProduct(m_player->GetActorUpVector(), forward);
  side.Normalize();

  FRotator rot = FRotationMatrix::MakeFromXY(forward, side).Rotator();
  this->SetActorRotation(rot);
}