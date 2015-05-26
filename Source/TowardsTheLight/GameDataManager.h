// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "rapidjson/document.h"

struct LevelData{
  int id = -1;
  FString name = "";
  unsigned int orbs = 0;
  float points = 0;
};

/**
 * 
 */
class TOWARDSTHELIGHT_API GameDataManager {
public:
  static GameDataManager* Instance() { if (m_instance == 0) m_instance = new GameDataManager(); return m_instance; }
  void AllowWrite(bool allow) { m_allowWrite = allow; }

  bool SavedGame();

  LevelData ReadLevelData(int id);
  bool WriteLevelData(LevelData data);

protected:
  GameDataManager();
  ~GameDataManager();

private:
  FString m_filePath = "Content/StorageFiles/saved.json";
  static GameDataManager* m_instance;
  bool m_allowWrite;
  FString m_data;
};
