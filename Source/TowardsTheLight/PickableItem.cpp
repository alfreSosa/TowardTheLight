// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "PickableItem.h"


// Sets default values
APickableItem::APickableItem() {
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  this->SetActorEnableCollision(true);
<<<<<<< HEAD
=======
  Points = DEFAULT_POINTS;
<<<<<<< HEAD

>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24

=======
>>>>>>> acbe53ae9c50350e661dee4b0e097188643d2549
}

void APickableItem::ReceiveActorBeginOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player")) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item Picked")));
<<<<<<< HEAD
    //sumar y destruir
=======
    this->SetActorEnableCollision(false);
    SetActorLocation(FVector(0, 0, 0));
    AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (gameMode) {
      gameMode->AddPoints(this->Points);
      if (IsOrb)
        gameMode->OrbPicked();
    }
>>>>>>> 742ca054506ccc07176a70c0bf14b1bdde0feb24
  }
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay() {
	Super::BeginPlay();
	
}


