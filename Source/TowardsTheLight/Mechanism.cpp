// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Mechanism.h"
#include "MobilePlatform.h"
#include "TimeManager.h"

AMechanism::AMechanism()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMechanism::BeginPlay()
{
	Super::BeginPlay();
  m_mobileTarget = dynamic_cast<AMobilePlatform *>(Target);
  //prueba de concepto
  if (m_mobileTarget) {
    m_isEnabled = true;
    if (GEngine)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("He asignado una movil como referencia")));
  }

}

void AMechanism::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

}

void AMechanism::Activate(bool enabled) {

}

void AMechanism::Execute() {

}


