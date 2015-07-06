// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tappable.h"
#include "Altar.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AAltar : public ATappable
{
	GENERATED_BODY()
public:	
  UPROPERTY(EditAnywhere, Category = Altar)
    bool GiveKey;
  UPROPERTY(EditAnywhere, Category = Altar)
    FLinearColor AltarColor;

  AAltar();
  virtual void BeginPlay() override;
  //virtual void Tick(float DeltaSeconds) override;
  virtual void Activate(bool enabled);
  virtual void Execute();
  UMaterialInstanceDynamic *AltarMaterial;
};
