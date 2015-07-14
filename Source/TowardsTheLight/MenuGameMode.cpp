// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MenuGameMode.h"
#include "GameDataManager.h"


float AMenuGameMode::GetLevelOrbs(FString levelName){
  return GameDataManager::Instance()->GetOrbsLevel(levelName);
}

float AMenuGameMode::GetLevelPoints(FString levelName){
  return GameDataManager::Instance()->GetPointsLevel(levelName);
}

float AMenuGameMode::GetTotalOrbs(){
  return GameDataManager::Instance()->GetOrbsCounts();
}

bool AMenuGameMode::LevelExists(FString levelName){
  return GameDataManager::Instance()->LevelExists(levelName);
}

bool AMenuGameMode::HasMusicBP(){
  return GameDataManager::Instance()->HasMusic();
}

void AMenuGameMode::SetMusicBP(bool enable){
  GameDataManager::Instance()->SetMusic(enable);
}

bool AMenuGameMode::HasEffectsBP(){
  return GameDataManager::Instance()->HasEffects();
}

void AMenuGameMode::SetEffectsBP(bool enable){
  GameDataManager::Instance()->SetEffects(enable);
}