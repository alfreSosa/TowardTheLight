// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "InfoGameInstance.h"


UInfoGameInstance::UInfoGameInstance(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
  m_galaxyName = "";
}

void UInfoGameInstance::SetCurrentGalaxy(FString galaxyName){
  m_galaxyName = galaxyName;
}
FString UInfoGameInstance::GetCurrentGalaxy(){
  return m_galaxyName;
}

