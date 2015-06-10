// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tappable.h"
#include "Mechanism.generated.h"

class AMobilePlatform;

UCLASS()
class TOWARDSTHELIGHT_API AMechanism : public ATappable
{
	GENERATED_BODY()
	
public:
  //Properties
  UPROPERTY(EditAnywhere, Category = MechanismTarget)
    AActor *Target;
	//constructor
	AMechanism();

	//Class Functions
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  virtual void Activate(bool enabled);
  virtual void Execute();

private:
  //MechanismState
  bool m_isEnabled;
  //TargetVariables
  AMobilePlatform *m_mobileTarget;
	
};
