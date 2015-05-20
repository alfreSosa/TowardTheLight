// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "FileManager.h"
=======
#include "GameDataManager.h"
>>>>>>> acbe53ae9c50350e661dee4b0e097188643d2549
#include "rapidjson/rapidjson.h"

FString FilePath = "Content/Levels/Levels.json";
>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  PrimaryActorTick.bCanEverTick = true;

  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();
<<<<<<< HEAD
  
=======
  m_countOrbs = m_actualPoints = 0;
  GameDataManager::Instance()->ReadData();
  if (GEngine)
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GameDataManager::Instance()->GetData()); //lee el fichero
}

void AMyGameMode::AddPoints(float points) {
  m_actualPoints += points;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));
}

void AMyGameMode::OrbPicked() {
  m_countOrbs++;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %f"), m_countOrbs));
<<<<<<< HEAD

>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24
=======
>>>>>>> acbe53ae9c50350e661dee4b0e097188643d2549
}

void AMyGameMode::EndGame(EndGameType type) {
  switch (type){
  case VICTORY:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("VICTORY!!!!")));

    break;
  case DEFEAT:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DEFEAT!!!!")));

    break;
  case WITHDRAWAL:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WITHDRAWAL!!!!")));

    break;
  }
}
