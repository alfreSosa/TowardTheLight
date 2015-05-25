// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "GameDataManager.h"
#include "CoreMisc.h"

using namespace rapidjson;

GameDataManager* GameDataManager::m_instance = nullptr;

GameDataManager::GameDataManager() {
  m_allowWrite = true;

  m_filePath = FPaths::GameDir() + m_filePath;
  FFileHelper::LoadFileToString(m_data, *m_filePath);
}

GameDataManager::~GameDataManager() {
  SavedGame();
  m_data = "";
  m_allowWrite = false;
}

bool GameDataManager::SavedGame() {
  if (m_allowWrite){
    //copiar en m_data el contenido de m_doc

    //FFileHelper::LoadFileToString(m_data, *m_filePath); Escritura
    return true;
  }
  return false;
}


//al llamar a esta función, hay que comprobar si el id del struct devuelto coincide con el id que se pasó como parámetro
LevelData GameDataManager::ReadLevelData(int id){
  LevelData ret;
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
              if (levels[i].HasMember("id"))
                if (levels[i]["id"].IsInt())
                  if (levels[i]["id"].GetInt() == id){
                    ret.id = id;

                    if (levels[i].HasMember("name"))
                      if (levels[i]["name"].IsString())
                        ret.name = levels[i]["name"].GetString();

                    if (levels[i].HasMember("orbs"))
                      if (levels[i]["orbs"].IsInt())
                        ret.orbs = levels[i]["orbs"].GetInt();

                    if (levels[i].HasMember("points"))
                      if (levels[i]["points"].IsDouble())
                        ret.points = levels[i]["points"].GetDouble();

                    return ret;
                  }
        }

  return ret;
}


bool GameDataManager::WriteLevelData(LevelData data){
  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*m_data));

  //escribir los nuevos valores en doc

  //copiar la variable doc en m_data

  return false;
}
