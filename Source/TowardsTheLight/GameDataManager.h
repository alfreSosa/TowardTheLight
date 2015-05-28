// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

struct LevelData{
  FString name = "";
  float orbs = 0;
  float points = 0;
};

/**
 * https://answers.unrealengine.com/questions/205899/c-class-singleton-problem.html
 */
class TOWARDSTHELIGHT_API GameDataManager {
public:
  static GameDataManager* Instance();

  bool SavedGame();

  LevelData ReadLevelData(FString levelName);
  void WriteLevelData(LevelData data);

protected:
  GameDataManager();
  ~GameDataManager();

private:
  static GameDataManager* m_instance;

  FString m_filePath = "StorageFiles/save/save.json";
  FString m_data;
};
