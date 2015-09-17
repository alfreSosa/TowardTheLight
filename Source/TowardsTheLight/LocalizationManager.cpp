// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "LocalizationManager.h"
#include "GameDataManager.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <string>

using namespace rapidjson;

//LangDef
const FString& LocalizationManager::LangDef::GetString(const FString& key, const FString& defVal) const {
  unsigned size = m_keys.Num();
  for (unsigned i = 0; i < size; i++)
    if (m_keys[i] == key)
      return m_values[i];
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
  ParseLanguage("en");
  ParseLanguage("es");

  SetLanguage(GameDataManager::Instance()->GetLanguage());
}

LocalizationManager::~LocalizationManager() {
  delete m_instance;
  m_instance = nullptr;
}

bool LocalizationManager::ParseLanguage(const FString& language) {
  FString path = FPaths::GameContentDir() + "Languages/" + language + ".json";
  FString buffer;
  FFileHelper::LoadFileToString(buffer, *path);

  //std::string s = TCHAR_TO_ANSI(*buffer);

  //const char a[] = {-31, 0};

//  while (s.find("#") != std::string::npos) {
//    s.replace(s.find("#"), 1, (char*)a);
//  }

  Document doc;
  //doc.Parse<0>(s.c_str());
  doc.Parse<0>(TCHAR_TO_ANSI(*buffer));

  if (!doc.HasParseError())
    if (doc.IsObject())
      if (doc.HasMember("language"))
        if (doc["language"].IsString()){
          FString langName = doc["language"].GetString();
          LangDef langDef(langName);

          if (doc.HasMember("keys")){
            if (doc["keys"].IsArray()){
              SizeType numKeys = doc["keys"].Size();
              const Value &keys = doc["keys"];
              for (SizeType i = 0; i < numKeys; i++)
                if (keys[i].IsObject())
                  if (keys[i].HasMember("key"))
                    if (keys[i]["key"].IsString()){
                      FString k = keys[i]["key"].GetString();
                      if (keys[i].HasMember("value"))
                        if (keys[i]["value"].IsString()){
             //             const char *c = keys[i]["value"].GetString();

              //            wchar_t cc[1024];
             //             mbstowcs(cc, c, 1024);

             //             FString v = cc;
                          FString v = keys[i]["value"].GetString();

              //            if (k == "RRR")
             //             GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, v);

                          v = ChangeSpecialSimbols(v);

            //              char *c2 = TCHAR_TO_ANSI(*v);
                          langDef.AddString(k, v);
                        }
                    }
            }
            m_languages.Add(langDef);

            return true;
          }
        }

  return false;
}

bool LocalizationManager::SetLanguage(const FString& language) {
  unsigned size = m_languages.Num();
  for (unsigned i = 0; i < size; i++)
    if (m_languages[i].GetName() == language) {
      m_currentLanguage = i;
      return true;
    }
  return false;
}

FString LocalizationManager::ChangeSpecialSimbols(FString s){
  //wchar_t cc[5];
  //mbstowcs(cc,"á", 5);
  //s.ReplaceInline(TEXT("#"), cc);

  //s.ReplaceInline(TEXT("#"), L"á");


  const char a[] = { -31, 0 };
  wchar_t aa[2];
  mbstowcs(aa, a, 2);
  const char b[] = { '#', 0 };
  wchar_t bb[2];
  mbstowcs(bb, b, 2);
  s.ReplaceInline(bb, aa);
  //s.ReplaceInline(TEXT("$"), TEXT("é"));
  //s.ReplaceInline(TEXT("%"), TEXT("í"));
  //s.ReplaceInline(TEXT("'"), TEXT("ó"));
  //s.ReplaceInline(TEXT("*"), TEXT("ú"));



  //s.ReplaceInline(TEXT("+"), TEXT("ñ"));


  //s.ReplaceInline(TEXT("("), TEXT("¡"));
  //s.ReplaceInline(TEXT(")"), TEXT("¿"));
  //s.ReplaceInline(TEXT("^"), TEXT("Á"));
  //s.ReplaceInline(TEXT(";"), TEXT("É"));
  //s.ReplaceInline(TEXT("<"), TEXT("Í"));
  //s.ReplaceInline(TEXT("="), TEXT("Ó"));
  //s.ReplaceInline(TEXT(">"), TEXT("Ú"));
  //s.ReplaceInline(TEXT("@"), TEXT("Ñ"));

  return s;
}
