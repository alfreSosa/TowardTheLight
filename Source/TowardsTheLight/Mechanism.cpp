// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Mechanism.h"
#include "MobilePlatform.h"
#include "TimeManager.h"

AMechanism::AMechanism()
{
	PrimaryActorTick.bCanEverTick = true;
  CanActivate = CanDisactivate = true;
  DisableAtEndAction = false;
  NumberOfActions = 1;
}

void AMechanism::BeginPlay()
{
	Super::BeginPlay();
  m_mobileTarget = dynamic_cast<AMobilePlatform *>(Target);
  //prueba de concepto
  if (m_mobileTarget) {
    m_isEnabled = true;
    m_mobileTarget->InitByMechanism(DisableAtEndAction, NumberOfActions);
  }

}

void AMechanism::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

}

void AMechanism::Activate(bool enabled) {

}

void AMechanism::Execute() {
  if (m_isEnabled) {
    if (m_mobileTarget) {
      if (m_mobileTarget->isEnabled()) {
        if (CanDisactivate) {
          m_mobileTarget->ChangeEnabled(false);
          GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MobilePlatform Desactivated")));
        }
      }
      else {
        if (CanActivate) {
          m_mobileTarget->ChangeEnabled(true);
          GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MobilePlatform Activated")));
        }
      }

    }
  }
}


