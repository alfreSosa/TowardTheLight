// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tappable.h"
#include "Mechanism.generated.h"

class AStaticPlatform;

UCLASS()
class TOWARDSTHELIGHT_API AMechanism : public ATappable
{
	GENERATED_BODY()
	
public:
  //Properties
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    TArray<AActor *> Targets;
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    bool TargetsAreEnabled;
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    bool CanActivate;
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    bool CanDisactivate;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    bool DisableAtEndAction;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    int32 NumberOfActions;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    float TimeInIntermittence;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    float TimeToStartIntermittence;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    FLinearColor ColorDisabled;
  UPROPERTY(EditAnywhere, Category = MechanismResponse)
    FLinearColor ColorEnabled;
	//constructor
	AMechanism();

	//Class Functions
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  virtual void Activate(bool enabled);
  virtual void Execute();

private:
  //TargetVariables
  float m_elapsedIntermitence;
  float m_elapsedStartIntermitence;
  bool intermitedOn;
  FLinearColor m_target;
  FLinearColor m_origin;
  TArray<AStaticPlatform *> m_Targets;
  UMaterialInstanceDynamic *MechanismMaterial;
};
