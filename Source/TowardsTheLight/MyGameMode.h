// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
  enum EndGameType { VICTORY, DEFEAT, WITHDRAWAL };

  AMyGameMode(const FObjectInitializer& ObjectInitializer);  // Our added constructor

  UFUNCTION()
    void AddPoints(float points);
  UFUNCTION()
    void OrbPicked();

  void EndGame(EndGameType type);

  bool SaveJason(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting);
  bool LoadJason(FString SaveDirectory, FString FileName, FString SaveText, bool AllowOverWriting);
private:
  float m_actualPoints;
  unsigned int m_countOrbs;
};

