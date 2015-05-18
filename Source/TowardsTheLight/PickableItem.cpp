// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "PickableItem.h"
#include "MyGameMode.h"



// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  this->SetActorEnableCollision(true);
  Points = DEFAULT_POINTS;


}

void APickableItem::ReceiveActorBeginOverlap(AActor* OtherActor)
{
  if (OtherActor->ActorHasTag("Player")) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item Picked")));
    this->SetActorEnableCollision(false);
    //PrimaryActorTick.bCanEverTick = false;
    SetActorLocation(FVector(0, 0, 0));
    AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (gameMode) {
      gameMode->AddPoints(this->Points);
      if (IsOrb)
        gameMode->OrbPicked();

    }
  }
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

//// Called every frame
//void APickableItem::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}

