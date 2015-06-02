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

  static ConstructorHelpers::FObjectFinder<UBlueprint> SomeBlueprint(TEXT("Blueprint'/Game/GameElementsBP/LevelHUD.LevelHUD'"));
  if (SomeBlueprint.Object)
    HUDClass = (UClass*)SomeBlueprint.Object->GeneratedClass;

  m_countOrbs = m_actualPoints = 0;
}

void AMyGameMode::AddPoints(float points) {
  m_actualPoints += points;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));

  SetPauseBP(true);
}

void AMyGameMode::OrbPicked() {
  m_countOrbs++;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %d"), m_countOrbs));
}

void AMyGameMode::EndGame(EndGameType type) {
  switch (type){
  case VICTORY:{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("VICTORY!!!!")));

    LevelData data = GameDataManager::Instance()->ReadLevelData(GetWorld()->GetMapName());
    //si la puntuacion actual es mejor que la que hay en el fichero, hay que almacenarla 
    bool write = false;
    if (m_countOrbs > data.orbs){
      data.orbs = m_countOrbs;
      write = true;
    }
    if (m_actualPoints > data.points){
      data.points = m_actualPoints;
      write = true;
    }
    if (write)
      GameDataManager::Instance()->WriteLevelData(data);

    //terminar la partida. volver al menú
    GameVictory();
  }
    break;
  case DEFEAT:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DEFEAT!!!!")));

    //terminar la partida. volver al menú
    GameDefeat();
    break;
  case WITHDRAWAL:
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WITHDRAWAL!!!!")));

    //terminar la partida. volver al menú
    GameWithdrawal();
    break;
  }
}

float AMyGameMode::GetActualPoints() {
  return m_actualPoints; 
}

float AMyGameMode::GetActualOrbs() {
  return m_countOrbs; 
}

void AMyGameMode::SetPauseBP(bool enable) {
  AOviPlayerController* const PlayerController = (AOviPlayerController*)GetWorld()->GetFirstPlayerController();
  if (PlayerController != NULL)
    PlayerController->SetPause(enable);
}

bool AMyGameMode::IsPausedBP() {
  AOviPlayerController* const PlayerController = (AOviPlayerController*)GetWorld()->GetFirstPlayerController();
  if (PlayerController != NULL)
    return PlayerController->IsPaused();
  return false;
}