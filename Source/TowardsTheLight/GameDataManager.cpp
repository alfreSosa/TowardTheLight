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
  m_data = "";
  m_filePath = FPaths::GameContentDir() + m_filePath;

  //solucion normal
  //FFileHelper::LoadFileToString(m_data, *m_filePath);
  
  //solucion con FILE*
  //FILE * pFile;
  //fopen_s(&pFile, TCHAR_TO_ANSI(*m_filePath), "r");

  //if (pFile){
  //  char buffer[256];
  //  while (!feof(pFile)){
  //    if (fgets(buffer, 256, pFile) == NULL) break;
  //    m_data = m_data + FString(buffer);
  //  }
  //  fclose(pFile);
  //}
  //else{
  //  FString content("{\"general\":{\"sound\":true}, \"levels\" : []}");
  //  m_data = content;
  //  SavedGame();
  //  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GUARDANDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO")));
  //}

  //solucion con parseo
  FFileHelper::LoadFileToString(m_data, *m_filePath);
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  if (doc.HasParseError()){
    ResetGame();
  }
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

void GameDataManager::ResetGame() {
  FString initial_content;
  FString initial_content_path = FPaths::GameContentDir() + "StorageFiles/init/initial.json";
  FFileHelper::LoadFileToString(initial_content, *initial_content_path);

  m_data = initial_content;
  SavedGame();
}

LevelData GameDataManager::ReadLevelData(FString levelName){
  LevelData ret;
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  //Quitar el inicio por defecto
  levelName.RemoveFromStart(FString("UEDPIE_0_"));

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

float GameDataManager::GetOrbsCounts() {
  float ret = 0;
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("levels"))
        if (doc["levels"].IsArray()){
          SizeType numLevels = doc["levels"].Size();
          const Value &levels = doc["levels"];
          for (SizeType i = 0; i < numLevels; i++)
            if (levels[i].IsObject())
              if (levels[i].HasMember("orbs"))
                if (levels[i]["orbs"].IsNumber())
                  ret += levels[i]["orbs"].GetDouble();
        }

  return ret;
}