// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "LocalizationManager.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

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
