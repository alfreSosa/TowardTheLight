// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "InfoGameInstance.h"
//#include "SoundManager.h"


UInfoGameInstance::UInfoGameInstance(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
  m_galaxyName = "";

  /*

  static ConstructorHelpers::FObjectFinder<USoundCue> soundLoader(TEXT("SoundCue'/Game/Sounds/Menu_track_Cue.Menu_track_Cue'"));
  USoundCue* m_pruebaCue = soundLoader.Object;

  //static ConstructorHelpers::FObjectFinder<USoundWave> soundLoader(TEXT("SoundWave'/Game/Sounds/Menu_track.Menu_track'"));
  //USoundWave* m_pruebaCue = soundLoader.Object;

  SoundManager::Instance()->SetSound(m_pruebaCue);

  */
}

void UInfoGameInstance::SetCurrentGalaxy(FString galaxyName){
  m_galaxyName = galaxyName;
}
FString UInfoGameInstance::GetCurrentGalaxy(){
  return m_galaxyName;
}

