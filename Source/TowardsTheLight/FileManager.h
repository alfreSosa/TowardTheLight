
#pragma once

#include "TowardsTheLight.h"


class FileManager {
public:
  const FString FILE_PATH = "Content/Levels/Levels.json";
  
  static FileManager* Instance() { if (mInstance == 0) mInstance = new FileManager(); return mInstance; }
protected:
  FileManager() { m_data = ""; }
  ~FileManager() { }
  void ReadData();
  void WriteData(FString data);
  FString GetData() { return m_data; }
private:
  static FileManager* mInstance;
  FString m_data;
};
