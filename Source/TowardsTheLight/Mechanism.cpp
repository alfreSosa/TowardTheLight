// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Mechanism.h"
#include "StaticPlatform.h"
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
  int32 numTargets = Targets.Num();
  m_Targets.Init(numTargets);
  for (int32 i = 0; i < numTargets; i++)
    m_Targets[i] = dynamic_cast<AStaticPlatform *>(Targets[i]);

  for (int32 i = 0; i < numTargets; i++)
    m_Targets[i]->InitByMechanism(DisableAtEndAction, NumberOfActions);
}

void AMechanism::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

}

void AMechanism::Activate(bool enabled) {

}

void AMechanism::Execute() {
  int32 numTargets = m_Targets.Num();
  for (int32 i = 0; i < numTargets; i++) {
    if (m_Targets[i]->isEnabled()) {
      if (CanDisactivate) {
        m_Targets[i]->ChangeEnabled(false);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Platform Desactivated")));
      }
    }
    else {
      if (CanActivate) {
        m_Targets[i]->ChangeEnabled(true);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Platform Activated")));
      }
    }
  }
}


