// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "StaticPlatform.h"

AStaticPlatform::AStaticPlatform()
{
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  RootComponent->SetMobility(EComponentMobility::Static);
  OurVisibleComponent->SetMobility(EComponentMobility::Static);
  OurVisibleComponent->AttachTo(RootComponent);
  this->Tags.Add("Platform");
}

