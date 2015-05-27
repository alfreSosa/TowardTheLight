// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "GameDataManager.h"
#include "CoreMisc.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

GameDataManager* GameDataManager::m_instance = nullptr;

GameDataManager* GameDataManager::Instance() {
  if (m_instance == nullptr)
    m_instance = new GameDataManager();
  return m_instance;
}

GameDataManager::GameDataManager() {
  m_filePath = FPaths::GameContentDir() + m_filePath;
  bool open = FFileHelper::LoadFileToString(m_data, *m_filePath);
  
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FULL_PATH")));
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, m_filePath);
  FPaths:
  if (open)
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OPEN")));

  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FEATURE_PACK_DIR")));
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FPaths::FeaturePackDir());
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Automatic_DIR")));
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FPaths::AutomationDir());
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GameContentDir")));
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FPaths::GameContentDir());


}

GameDataManager::~GameDataManager() {
  SavedGame();

  m_data = "";
  delete m_instance;
  m_instance = nullptr;
}

bool GameDataManager::SavedGame() {
  return FFileHelper::SaveStringToFile(m_data, *m_filePath);
}

LevelData GameDataManager::ReadLevelData(FString levelName){
  LevelData ret;
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  //Quitar el inicio por defecto
  levelName.RemoveFromStart(FString("UEDPIE_0_"));

  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FULL_PATH")));
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, m_filePath);

  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("READ_LEVEL_DATA")));
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, m_data);

  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("levels"))
        if (doc["levels"].IsArray()){
          SizeType numLevels = doc["levels"].Size();
          const Value &levels = doc["levels"];
          for (SizeType i = 0; i < numLevels; i++)
            if (levels[i].IsObject())
              if (levels[i].HasMember("name"))
                if (levels[i]["name"].IsString())
                  if (FString(levels[i]["name"].GetString()) == levelName){
                    ret.name = levels[i]["name"].GetString();

                    if (levels[i].HasMember("orbs"))
                      if (levels[i]["orbs"].IsNumber())
                        ret.orbs = levels[i]["orbs"].GetDouble();

                    if (levels[i].HasMember("points"))
                      if (levels[i]["points"].IsNumber())
                        ret.points = levels[i]["points"].GetDouble();

                    return ret;
                  }
        }
  
  // si no lo ha encontrado en el fichero, devuelve un struct con el nombre y el resto de parámetros por defecto.
  // cuando se llame a la función WriteLevelData, si no existe el bloque, se añadirá al fichero
  ret.name = levelName;
  return ret;
}



void GameDataManager::WriteLevelData(LevelData data){
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  bool success = false;
  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("levels"))
        if (doc["levels"].IsArray()){
          SizeType numLevels = doc["levels"].Size();
          Value &levels = doc["levels"];
          for (SizeType i = 0; i < numLevels; i++){
            if (levels[i].IsObject())
              if (levels[i].HasMember("name"))
                if (levels[i]["name"].IsString())
                  if (FString(levels[i]["name"].GetString()) == data.name){

                    if (levels[i].HasMember("orbs"))
                      if (levels[i]["orbs"].IsNumber())
                        levels[i]["orbs"].SetDouble(data.orbs);

                    if (levels[i].HasMember("points"))
                      if (levels[i]["points"].IsNumber())
                        levels[i]["points"].SetDouble(data.points);

                    success = true;
                    break;
                  }
          }


          if (!success){ //añadir el nuevo bloque
            Document::AllocatorType& allocator = doc.GetAllocator();

            Value lvl(kObjectType);
            char* n = TCHAR_TO_ANSI(*data.name);
            lvl.AddMember("name", n, allocator);
            lvl.AddMember("orbs", data.orbs, allocator);
            lvl.AddMember("points", data.points, allocator);     

            levels.PushBack(lvl, allocator);
          }
        }

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  doc.Accept(writer);
  m_data = buffer.GetString();

  SavedGame();
}
