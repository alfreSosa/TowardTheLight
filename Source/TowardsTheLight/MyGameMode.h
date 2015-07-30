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
  enum EndGameType { NONE = 0, VICTORY = 1, DEFEAT = -1 };

  AMyGameMode(const FObjectInitializer& ObjectInitializer);  // Our added constructor

  UFUNCTION()
    void AddPoints(float points);
  UFUNCTION()
    void OrbPicked();
  
  UFUNCTION(BlueprintCallable, Category = "GameState")
    float GetActualPoints();
  UFUNCTION(BlueprintCallable, Category = "GameState")
    float GetActualOrbs();

  void EndGame(EndGameType type);

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    void SetPauseBP(bool enable);

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    bool IsPausedBP();

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    float EndGameBP();

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    FString GetLevelNameBP();

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    FString GetString(FString key);


  UFUNCTION(BlueprintImplementableEvent, Category = TTLEvents)
    void PointsSoundEvent();

  UFUNCTION(BlueprintImplementableEvent, Category = TTLEvents)
    void OrbsSoundEvent();

private:
  float m_actualPoints;
  float m_countOrbs;

  EndGameType state;
};

