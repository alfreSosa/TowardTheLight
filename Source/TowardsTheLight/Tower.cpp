// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tower.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"


// Sets default values
ATower::ATower() {
  RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
  RootComponent->SetWorldScale3D(FVector(2.5, 2.5, 2.5));
  RootComponent = Body;
  this->Tags.Add("Platform");

  Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
  Entrance->AttachTo(RootComponent);

  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->AttachTo(Entrance);
  Trigger->bHiddenInGame = true;
  Trigger->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
  Trigger->bGenerateOverlapEvents = true;

  NeedKey = false;
  ColorKey = FLinearColor(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
	
  RegisterDelegate();
}

void ATower::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
}

void ATower::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    APlayerOvi *player = dynamic_cast<APlayerOvi *>(OtherActor);
    if (player){
      FVector dif = player->GetActorUpVector() - Entrance->GetUpVector();
      dif.X = (dif.X < 0) ? -dif.X : dif.X;
      dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
      dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
      if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05){
        if (!NeedKey || (NeedKey && player->HasKey() && ColorKey == player->GetColorKey())){
          AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
          if (gameMode)
            gameMode->EndGame(AMyGameMode::VICTORY);
        }
      }
    }
  }
}

void ATower::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}