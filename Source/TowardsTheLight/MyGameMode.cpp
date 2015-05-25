// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
#include "GameDataManager.h"

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  PrimaryActorTick.bCanEverTick = true;

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

void AMyGameMode::EndGame(EndGameType type) {
  switch (type){
  case VICTORY:{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("VICTORY!!!!")));

    int ID = 0;
    LevelData data = GameDataManager::Instance()->ReadLevelData(ID);
    //if (GEngine)
    //  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GameDataManager::Instance()->GetData()); //lee el fichero

    //si la puntuacion actual es mejor que la que hay en el fichero, hay que almacenarla

  }
    break;
  case DEFEAT:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DEFEAT!!!!")));

    break;
  case WITHDRAWAL:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WITHDRAWAL!!!!")));

    break;
  }
}
