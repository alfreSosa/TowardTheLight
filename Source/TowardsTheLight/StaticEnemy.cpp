// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "StaticEnemy.h"
#include "MyGameMode.h"

// Sets default values
AStaticEnemy::AStaticEnemy() {
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  this->SetActorEnableCollision(true);
}

// Called when the game starts or when spawned
void AStaticEnemy::BeginPlay() {
	Super::BeginPlay();
	
}

void AStaticEnemy::ReceiveActorBeginOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player")) {
    AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (gameMode) {
      gameMode->EndGame(AMyGameMode::DEFEAT);
    }
  }
}