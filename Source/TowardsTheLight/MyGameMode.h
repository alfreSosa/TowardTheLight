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

  UFUNCTION(BlueprintImplementableEvent, Category = GameEvent)
    void BackEvent();

  UFUNCTION()
    void SetBackEvent();


private:
  float m_actualPoints;
  float m_countOrbs; //BUGS PARA BP, NO PUEDEN USAR INT, INVESTIGAR
};

