// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "GameDataManager.h"
#include "CoreMisc.h"

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
  m_filePath = FPaths::GameDir() + m_filePath;
  FFileHelper::LoadFileToString(m_data, *m_filePath);
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
                      if (levels[i]["orbs"].IsInt())
                        ret.orbs = levels[i]["orbs"].GetInt();

                    if (levels[i].HasMember("points"))
                      if (levels[i]["points"].IsNumber())
                        ret.points = levels[i]["points"].GetDouble();

                    return ret;
                  }
        }

  return ret;
}


bool GameDataManager::WriteLevelData(LevelData data){
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  bool success = false;
  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("levels"))
        if (doc["levels"].IsArray()){
          SizeType numLevels = doc["levels"].Size();
          Value &levels = doc["levels"];
          for (SizeType i = 0; i < numLevels; i++)
            if (levels[i].IsObject())
              if (levels[i].HasMember("name"))
                if (levels[i]["name"].IsString())
                  if (FString(levels[i]["name"].GetString()) == data.name){

                    if (levels[i].HasMember("orbs"))
                      if (levels[i]["orbs"].IsInt())
                        levels[i]["orbs"].SetInt(data.orbs);

                    if (levels[i].HasMember("points"))
                      if (levels[i]["points"].IsNumber())
                        levels[i]["points"].SetDouble(data.points);

                    success = true;
                    break;
                  }
        }

  if (success){
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    m_data = buffer.GetString();

    return SavedGame();
  }
  return false;
}
