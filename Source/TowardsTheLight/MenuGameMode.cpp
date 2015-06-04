// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MenuGameMode.h"
#include "GameDataManager.h"


FString AMenuGameMode::GetLevelOrbs(FString levelName){
  FString ret;
  ret = ret.FromInt(GameDataManager::Instance()->GetOrbsLevel(levelName));
  return ret;
}

FString AMenuGameMode::GetLevelPoints(FString levelName){
  FString ret;
  ret = ret.FromInt(GameDataManager::Instance()->GetPointsLevel(levelName));
  return ret;
}
