// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticPlatform.h"
#include "IntermittentManager.generated.h"

class AIntermittentPlatform;
UCLASS()
class TOWARDSTHELIGHT_API AIntermittentManager : public AStaticPlatform
{
	GENERATED_BODY()
	
public:	
  //properties
  UPROPERTY(EditAnywhere, Category = PlatformManaged)
    TArray<AIntermittentPlatform *> Platforms;

  //functions
	AIntermittentManager();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:
  //properties
  int32 m_numPlatforms;
};
