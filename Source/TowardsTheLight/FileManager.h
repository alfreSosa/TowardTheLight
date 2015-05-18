
#pragma once

#include "GameFramework/Actor.h"

class FileManager {
public:
  const FString FILE_PATH = "Content/Levels/Levels.json";
  
  static FileManager* Instance() { if (mInstance == 0) mInstance = new FileManager(); return mInstance; }
  void AllowWrite(bool allow) { m_allowWrite = allow; }
  void ReadData();
  bool WriteData(FString data);
  FString GetData() { return m_data; }
protected:
  FileManager() { m_data = ""; m_allowWrite = true; }
  ~FileManager() { }
private:
  static FileManager* mInstance;
  bool m_allowWrite;
  FString m_data;
};
