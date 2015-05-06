// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tower.h"

// Sets default values
ATower::ATower() {
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
  Body->AttachTo(RootComponent);

  Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
  Entrance->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void ATower::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

}

