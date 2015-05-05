// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "StaticPlatform.h"
#include "PlayerOvi.h"

AStaticPlatform::AStaticPlatform()
{
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AStaticPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticPlatform::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


