// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API AMenuGameMode : public AGameMode {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, Category = TTLMenuFunctions)
    FString GetLevelPoints(FString levelName);
  UFUNCTION(BlueprintCallable, Category = TTLMenuFunctions)
    FString GetLevelOrbs(FString levelName);

  UFUNCTION(BlueprintCallable, Category = TTLMenuFunctions)
    bool LevelExists(FString levelName);

  //UFUNCTION(BlueprintCallable, Category = TTLMenuFunctions)
  //  bool IsSwipeControlBP();
  //UFUNCTION(BlueprintCallable, Category = TTLMenuFunctions)
  //  void SetSwipeControlBP(bool enable);

private:
};
