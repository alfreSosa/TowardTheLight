// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "OviCharacter.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AOviCharacter : public ACharacter
{
	GENERATED_BODY()

public:
  enum States { STOP, RIGHT, LEFT };
  // Sets default values for this character's properties
  AOviCharacter();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  // Called every frame
  virtual void Tick(float DeltaSeconds) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  //sets right flag when key is pressed
  UFUNCTION()
    void OnStartRight();
  //clears right flag when key is released
  UFUNCTION()
    void OnStopRight();
  //sets left flag when key is pressed
  UFUNCTION()
    void OnStartLeft();
  //clears left flag when key is released
  UFUNCTION()
    void OnStopLeft();

private:
  bool m_right;
  bool m_left;
  bool m_isJumping;
  States m_state;
  float m_limit;
  float m_jumpDistance;
};
