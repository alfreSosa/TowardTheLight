// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Altar.h"
#include "PlayerOvi.h"

// Sets default values
AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);
  this->Tags.Add("Platform");

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->AttachTo(RootComponent);
  //trigger

  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationZone"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;

  GiveKey = true;
  ColorKey = FLinearColor(0.0f, 0.0f, 1.0f);
}

// Called when the game starts or when spawned
void AAltar::BeginPlay(){
	Super::BeginPlay();
  RegisterDelegate();
}

void AAltar::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AAltar::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AAltar::OnBeginTriggerOverlap);
  }
}

void AAltar::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){

    FVector dif = OtherActor->GetActorUpVector() - GetActorUpVector();
    dif.X = (dif.X < 0) ? -dif.X : dif.X;
    dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
    dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
    if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05){
      APlayerOvi *player = dynamic_cast<APlayerOvi *>(OtherActor);
      if (player)
        player->SetKey(GiveKey, ColorKey);
    }
    
  }
}

void AAltar::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AAltar::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &AAltar::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}

