// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Altar.h"


// Sets default values
AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);
  //this->Tags.Add("Platform");

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  //trigger
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationZone"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
}

// Called when the game starts or when spawned
void AAltar::BeginPlay()
{
	Super::BeginPlay();
  RegisterDelegate();
}

// Called every frame
void AAltar::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AAltar::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AAltar::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AAltar::OnBeginTriggerOverlap);
  }
}

void AAltar::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    if (GEngine)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ACTIVADA LLAVE");
  }
}

void AAltar::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AAltar::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &AAltar::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}

