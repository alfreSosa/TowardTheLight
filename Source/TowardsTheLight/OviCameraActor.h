// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include "OviCameraActor.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AOviCameraActor : public ACameraActor
{
  GENERATED_BODY()

  //const float MAX_CAMERA_SPEED = 100;

  class APlayerOvi *m_player;
  float m_limitWorld;
  float m_limitPadding;
  float m_cameraDistance;
  float m_ruleOfThree;
  //float m_deltaLerp;

  void SetPosition();
  void SetOrientation();
public:
  AOviCameraActor(const FObjectInitializer& ObjectInitializer);  // Our added constructor

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
  // Called every frame
  virtual void Tick(float DeltaSeconds) override;
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ConfigCamera)
    float Padding;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ConfigCamera)
    float DistanceCameraToPlayer;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ConfigCamera)
    float CameraSpeed;
};
