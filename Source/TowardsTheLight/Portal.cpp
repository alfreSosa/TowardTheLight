// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Portal.h"
#include "PlayerOvi.h"

void APortal::BeginPlay() {
  Super::BeginPlay();
}

void APortal::Activate(bool enabled) {
}

void APortal::Execute() {
  FVector dif = m_player->GetActorUpVector() - MeshActivator->GetUpVector();
  dif.X = (dif.X < 0) ? -dif.X : dif.X;
  dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
  dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
  if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05) {
    FTransform newTransformPlayer = Partner->GetTransform();

    newTransformPlayer.SetScale3D(m_player->GetTransform().GetScale3D());

    float limitWorld = FVector::DotProduct(m_player->GetActorLocation(), m_player->GetActorRightVector());
    limitWorld = abs(limitWorld);
    FVector pForward = Partner->GetActorForwardVector();
    FVector pUp = Partner->GetActorUpVector();

    //position
    FVector location = newTransformPlayer.GetLocation();

    if (pForward.X > 0.05 || pForward.X < -0.05)
      location.X = (location.X > 0) ? limitWorld : -limitWorld;
    if (pForward.Y > 0.05 || pForward.Y < -0.05)
      location.Y = (location.Y > 0) ? limitWorld : -limitWorld;
    if (pForward.Z > 0.05 || pForward.Z < -0.05)
      location.Z = (location.Z > 0) ? limitWorld : -limitWorld;

    newTransformPlayer.SetLocation(location);

    //rotation
    FQuat q = newTransformPlayer.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, (m_player->PlayerisToRight()) ? 90 : -90));
    
    newTransformPlayer.SetRotation(q);
    
    m_player->SetActorTransform(newTransformPlayer);
  }
}

