// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TOWARDSTHELIGHT_API GameDataManager
{

public:
  const FString FILE_PATH = "Content/Levels/Levels.json";
  static GameDataManager* Instance() { if (mInstance == 0) mInstance = new GameDataManager(); return mInstance; }
  void AllowWrite(bool allow) { m_allowWrite = allow; }
  void ReadData();
  bool WriteData(FString data);
  FString GetData() { return m_data; }
protected:
  GameDataManager();
  ~GameDataManager();
private:
  static GameDataManager* mInstance;
  bool m_allowWrite;
  FString m_data;
public:

};
