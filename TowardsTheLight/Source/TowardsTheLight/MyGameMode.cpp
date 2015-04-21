// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MyGameMode.h"
#include "OviCharacter.h"

AMyGameMode::AMyGameMode(const class FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  DefaultPawnClass = AOviCharacter::StaticClass();
}




