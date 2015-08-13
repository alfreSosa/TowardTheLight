// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "InfoGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API UInfoGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
  UInfoGameInstance(const FObjectInitializer& ObjectInitializer);

public:
  UFUNCTION(BlueprintCallable, Category = "GameInfo")
    void SetCurrentGalaxy(FString galaxyName);
  UFUNCTION(BlueprintCallable, Category = "GameInfo")
    FString GetCurrentGalaxy();

  UFUNCTION(BlueprintCallable, Category = "GameInfo")
    void SetCurrentLevel(FString levelName);
  UFUNCTION(BlueprintCallable, Category = "GameInfo")
    FString GetCurrentLevel();
	
private:
  FString m_galaxyName;
  FString m_levelName;
};
