// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "SoundManager.h"
#include "AudioDevice.h"

SoundManager* SoundManager::m_instance = nullptr;

SoundManager* SoundManager::Instance() {
  if (m_instance == nullptr)
    m_instance = new SoundManager();
  return m_instance;
}

SoundManager::SoundManager() {
  //static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader(TEXT("SoundCue'/Game/Sounds/Menu_track_Cue.Menu_track_Cue'"));
  //m_pruebaCue = soundCueLoader.Object;

}

SoundManager::~SoundManager() {
  delete m_instance;
  m_instance = nullptr;
}

//void SoundManager::SetSound(USoundWave* cue){
//  m_pruebaCue = cue;
//}

//void SoundManager::SetSound(USoundCue* cue){
//  m_pruebaCue = cue;
//}

void SoundManager::SetMusic(bool enable){
  FAudioDevice* Device = GEngine->GetAudioDevice();
  if (!Device){
    return;
  }
  for (TMap<USoundClass*, FSoundClassProperties>::TIterator It(Device->SoundClasses); It; ++It){
    USoundClass* SoundClass = It.Key();
    if (SoundClass && SoundClass->GetFullName().Find("Music") != INDEX_NONE){
      if (enable)
        SoundClass->Properties.Volume = 1.f;
      else
        SoundClass->Properties.Volume = 0.f;
    }
  }


  //if (enable){
  //  m_pruebaCue->VolumeMultiplier = 1.f;
  //}
  //else{
  //  m_pruebaCue->VolumeMultiplier = 0.f;
  //}

  //if (enable){
  //  m_pruebaCue->Volume = 0.25f;
  //}
  //else{
  //  m_pruebaCue->Volume = 0.f;
  //}
}

void SoundManager::SetEffects(bool enable){
  FAudioDevice* Device = GEngine->GetAudioDevice();
  if (!Device){
    return;
  }
  for (TMap<USoundClass*, FSoundClassProperties>::TIterator It(Device->SoundClasses); It; ++It){
    USoundClass* SoundClass = It.Key();
    if (SoundClass && SoundClass->GetFullName().Find("SFX") != INDEX_NONE){
      if (enable)
        SoundClass->Properties.Volume = 1.f;
      else
        SoundClass->Properties.Volume = 0.f;
    }
  }
}