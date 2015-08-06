// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TOWARDSTHELIGHT_API SoundManager {
public:
  static SoundManager* Instance();

  //void SetSound(USoundWave* cue);
  //void SetSound(USoundCue* cue);

  void SetMusic(bool enable);
  void SetEffects(bool enable);

protected:
	SoundManager();
	~SoundManager();

private:
  static SoundManager* m_instance;

  //USoundWave* m_pruebaCue;
  //USoundCue* m_pruebaCue;
};
