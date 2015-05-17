// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
#include "rapidjson/rapidjson.h"

FString FilePath = "Content/Levels/Levels.json";

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();
  m_countOrbs = m_actualPoints = 0;
}

void AMyGameMode::AddPoints(float points) {
  m_actualPoints += points;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));
}

void AMyGameMode::OrbPicked() {
  m_countOrbs++;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %f"), m_countOrbs));

}




