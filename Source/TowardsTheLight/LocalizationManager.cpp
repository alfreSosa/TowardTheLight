// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "LocalizationManager.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

//LangDef
const FString& LangDef::GetString(const FString& key, const FString& defVal) const {
  for (unsigned i = 0; i < mKeys.Num(); i++)
    if (mKeys[i] == key) return mValues[i];
  return defVal;
}

//LocalizationManager
LocalizationManager* LocalizationManager::m_instance = nullptr;

LocalizationManager* LocalizationManager::Instance() {
  if (m_instance == nullptr)
    m_instance = new LocalizationManager();
  return m_instance;
}

LocalizationManager::LocalizationManager() {
}

LocalizationManager::~LocalizationManager() {
  delete m_instance;
  m_instance = nullptr;
}

bool LocalizationManager::ParseLanguage(const FString& lenguage) {
  FString path = FPaths::GameContentDir() + "Languages/" + lenguage + ".json";
  FString buffer;
  FFileHelper::LoadFileToString(buffer, *path);

  Document doc;
  doc.Parse<0>(TCHAR_TO_ANSI(*buffer));

  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("keys"))
        if (doc["keys"].IsArray()){
          SizeType numKeys = doc["keys"].Size();
          const Value &keys = doc["keys"];
          for (SizeType i = 0; i < numKeys; i++)
            if (keys[i].IsObject())
              if (keys[i].HasMember("key"))
                if (keys[i]["key"].IsString()){
                  FString k = keys[i]["key"].GetString();
                  FString v;
                  if (keys[i].HasMember("value"))
                    if (keys[i]["value"].IsString()){
                      v = keys[i]["value"].GetString();
                    }

                  //añadir k y v

                }
        }


  //xml_node<>* langNode = doc.first_node("language");
  //String langName = langNode->first_attribute("name")->value();
  //LangDef langDef(langName);

  //xml_node<>* stringNode = langNode->first_node("string");
  //if (!stringNode)
  //  return false;
  //while (stringNode) {
  //  String key = stringNode->first_attribute("key")->value();
  //  String value = stringNode->first_attribute("value")->value();
  //  langDef.AddString(key, value);
  //  stringNode = stringNode->next_sibling("string");
  //}

  //mLanguages.Add(langDef);

  return true;
}

bool LocalizationManager::SetLanguage(const FString& name) {
  for (unsigned i = 0; i < mLanguages.Num(); i++)
    if (mLanguages[i].GetName() == name) {
      mCurrentLanguage = i;
      return true;
    }
  return false;
}




