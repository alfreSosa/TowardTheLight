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
  
  UFUNCTION(BlueprintCallable, Category = "GameState")
    float GetActualPoints();
  UFUNCTION(BlueprintCallable, Category = "GameState")
    float GetActualOrbs();

  void EndGame(EndGameType type);

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    void SetPauseBP(bool enable);

  UFUNCTION(BlueprintCallable, Category = TTLFunctions)
    bool IsPausedBP();

  UFUNCTION(BlueprintImplementableEvent, Category = TTLEvents)
    void GameVictory();

  UFUNCTION(BlueprintImplementableEvent, Category = TTLEvents)
    void GameDefeat();

  UFUNCTION(BlueprintImplementableEvent, Category = TTLEvents)
    void GameWithdrawal();

private:
  float m_actualPoints;
  float m_countOrbs; //BUGS PARA BP, NO PUEDEN USAR INT, INVESTIGAR
};

