// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include "OviCameraActor.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AOviCameraActor : public ACameraActor
{
  GENERATED_BODY()

  class APlayerOvi *m_player;
  float m_limit;
  float m_sqrt2, m_sqrt3;

  void setPosition();
  void setOrientation();
public:
  AOviCameraActor(const FObjectInitializer& ObjectInitializer);  // Our added constructor

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
  // Called every frame
  virtual void Tick(float DeltaSeconds) override;
	

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ConfigCamera)
  float Padding;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ConfigCamera)
  float CameraDistance;
};
