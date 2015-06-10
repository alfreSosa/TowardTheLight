// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
#include "GameDataManager.h"
#include "TimeManager.h"

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  PrimaryActorTick.bCanEverTick = true;

  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();

  static ConstructorHelpers::FObjectFinder<UBlueprint> SomeBlueprint(TEXT("Blueprint'/Game/UIElementsBP/LevelHUD.LevelHUD'"));
  if (SomeBlueprint.Object)
    HUDClass = (UClass*)SomeBlueprint.Object->GeneratedClass;

  m_countOrbs = m_actualPoints = 0;
  state = EndGameType::NONE;
}

void AMyGameMode::AddPoints(float points) {
  m_actualPoints += points;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));

  PointsSoundEvent();
}

void AMyGameMode::OrbPicked() {
  m_countOrbs++;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %d"), m_countOrbs));

  OrbsSoundEvent();
}

void AMyGameMode::EndGame(EndGameType type) {
  switch (type){
  case VICTORY:{
    if (state == EndGameType::NONE){
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
    }

    //terminar la partida. volver al menú
    state = EndGameType::VICTORY;
  }
    break;
  case DEFEAT:
    //terminar la partida. volver al menú
    state = EndGameType::DEFEAT;
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
  TimeManager::Instance()->SetPause(enable);
}

bool AMyGameMode::IsPausedBP() {
  return TimeManager::Instance()->IsPaused();
}

float AMyGameMode::EndGameBP() {
  return (float)state;
}

FString AMyGameMode::GetLevelNameBP(){
  FString levelName = GetWorld()->GetMapName();
  levelName.RemoveFromStart(FString("UEDPIE_0_"));
  return levelName;
}