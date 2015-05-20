// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "GameDataManager.h"
#include "CoreMisc.h"

GameDataManager* GameDataManager::mInstance = nullptr;

GameDataManager::GameDataManager()
{
  m_data = "";
  m_allowWrite = true;
}

GameDataManager::~GameDataManager()
{
  m_data = "";
  m_allowWrite = false;
}


void GameDataManager::ReadData() {

  FString GameDir = FPaths::GameDir();
  FString CompleteFilePath = GameDir + FILE_PATH;
  FFileHelper::LoadFileToString(m_data, *CompleteFilePath);
}

bool GameDataManager::WriteData(FString data) {
  if (m_allowWrite)
  {
    FString GameDir = FPaths::GameDir();
    FString CompleteFilePath = GameDir + FILE_PATH;
    m_data = data;
    FFileHelper::LoadFileToString(m_data, *CompleteFilePath);
    return true;
  }
  return false;
}