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
  if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05){
    //calcular bien la posicion
    FVector location = Partner->GetActorLocation();
    m_player->SetActorLocation(location);

    //calcular bien la rotacion y asegurar que si entra mirando a la derecha, sale mirando a la derecha (igual para la izquierda)
    FRotator rotation = Partner->GetActorRotation();
    m_player->SetActorRotation(rotation);
    
  }
}






