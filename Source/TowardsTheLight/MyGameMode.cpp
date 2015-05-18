// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
<<<<<<< HEAD
=======
#include "FileManager.h"
#include "rapidjson/rapidjson.h"

FString FilePath = "Content/Levels/Levels.json";
>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();
<<<<<<< HEAD
  
=======
  m_countOrbs = m_actualPoints = 0;
  FileManager::Instance()->ReadData();
  
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FileManager::Instance()->GetData()); //lee el fichero
}

void AMyGameMode::AddPoints(float points) {
  m_actualPoints += points;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));
}

void AMyGameMode::OrbPicked() {
  m_countOrbs++;
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %f"), m_countOrbs));

>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24
}




