// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "PickableItem.h"
#include "MyGameMode.h"

// Sets default values
APickableItem::APickableItem() {
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  RootComponent->SetMobility(EComponentMobility::Static);
  OurVisibleComponent->SetMobility(EComponentMobility::Static);
  OurVisibleComponent->AttachTo(RootComponent);
  
  this->SetActorEnableCollision(true);

  Points = DEFAULT_POINTS;
  m_collected = false;
}

void APickableItem::ReceiveActorBeginOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player") && !m_collected) {
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item Picked")));
    m_collected = true;  //auqnue el ojeto se destruya, es bueno dejarlo por si al frame siguiente la memoria no se ha liberado aún
    //this->SetActorEnableCollision(false); //comentar si se pone el destroy
    //SetActorLocation(FVector(0, 0, 0)); //comentar si se pone el destroy 
    AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (gameMode) {
      gameMode->AddPoints(this->Points);
      if (IsOrb)
        gameMode->OrbPicked();
    }
    this->Destroy();
  }
}

