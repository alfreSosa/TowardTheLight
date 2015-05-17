#include "FileManager.h"
#include "CoreMisc.h"

FileManager* FileManager::mInstance = nullptr;

void FileManager::ReadData() {

  FString GameDir = FPaths::GameDir();
  FString GetData();
  FString CompleteFilePath = GameDir + FILE_PATH;
  FFileHelper::LoadFileToString(m_data, *CompleteFilePath);
}
