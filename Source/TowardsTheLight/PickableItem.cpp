// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "PickableItem.h"


// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  this->SetActorEnableCollision(true);

}

void APickableItem::ReceiveActorBeginOverlap(AActor* OtherActor)
{
  if (OtherActor->ActorHasTag("Player")) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item Picked")));
    //sumar y destruir
  }
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

