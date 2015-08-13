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
  //constructor
  APortal();

  //Properties
  UPROPERTY(EditAnywhere, Category = Portal)
    AActor *Partner;
  UPROPERTY(EditAnywhere, Category = Portal)
    FLinearColor ColorDisabled;
  UPROPERTY(EditAnywhere, Category = Portal)
    FLinearColor ColorEnabled;
  UPROPERTY(EditAnywhere, Category = Portal)
    FLinearColor PortalColor;

  //Class Functions
  virtual void BeginPlay() override;
  virtual void Activate(bool enabled);
  virtual void Execute();

private:
  UMaterialInstanceDynamic *PortalMaterial;
  UMaterialInstanceDynamic *PortalMaterialBG;
  UMaterialInstanceDynamic *PortalMaterialEffects;
};
