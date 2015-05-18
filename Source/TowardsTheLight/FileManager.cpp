#include "TowardsTheLight.h"
#include "FileManager.h"
#include "CoreMisc.h"

FileManager* FileManager::mInstance = nullptr;

void FileManager::ReadData() {

  FString GameDir = FPaths::GameDir();
  FString CompleteFilePath = GameDir + FILE_PATH;
  FFileHelper::LoadFileToString(m_data, *CompleteFilePath);
}

bool FileManager::WriteData(FString data) {
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
