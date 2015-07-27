// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "IntermittentManager.h"
#include "IntermittentPlatform.h"
#include "TimeManager.h"

// Sets default values
AIntermittentManager::AIntermittentManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  m_numPlatforms = 0;
}

// Called when the game starts or when spawned
void AIntermittentManager::BeginPlay()
{
	Super::BeginPlay();
  m_numPlatforms = Platforms.Num();
  for (int32 i = 0; i < m_numPlatforms; i++)
    Platforms[i]->BeginPlay();
}

// Called every frame
void AIntermittentManager::Tick( float DeltaTime )
{
  DeltaTime = TimeManager::Instance()->GetDeltaTime(DeltaTime);
	Super::Tick( DeltaTime );

  for (int32 i = 0; i < m_numPlatforms; i++)
    Platforms[i]->Tick(DeltaTime);

}

