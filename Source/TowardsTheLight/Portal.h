// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tappable.h"
#include "Portal.generated.h"

/**
 * 
 */
UCLASS()
class TOWARDSTHELIGHT_API APortal : public ATappable
{
	GENERATED_BODY()
	
public:
  //Properties
  UPROPERTY(EditAnywhere, Category = Portal)
    AActor *Partner;
	
  //Class Functions
  virtual void BeginPlay() override;
  virtual void Activate(bool enabled);
  virtual void Execute();
};
