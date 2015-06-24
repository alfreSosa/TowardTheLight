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
  void DeleteGameData();

  LevelData ReadLevelData(FString levelName);
  void WriteLevelData(LevelData data);

  float GetOrbsCounts();
  float GetOrbsLevel(FString levelName);
  float GetPointsLevel(FString levelName);
  bool LevelExists(FString levelName);

//  bool IsSwipeControl();
//  void SetSwipeControl(bool enable);

protected:
  GameDataManager();
  ~GameDataManager();

private:
  static GameDataManager* m_instance;

  FString m_filePath = "StorageFiles/save/save.json";
  FString m_data;

//  enum SwipeControl { NONE = 0, SWIPE = 1, BUTTONS = -1 } m_swipeControl;
};
