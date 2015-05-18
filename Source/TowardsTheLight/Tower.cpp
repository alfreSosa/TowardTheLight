// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tower.h"
#include "MyGameMode.h"

// Sets default values
ATower::ATower() {
  RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
  RootComponent->SetWorldScale3D(FVector(2.5, 2.5, 2.5));
  RootComponent = Body;
  this->Tags.Add("Platform");

  Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
  Entrance->AttachTo(RootComponent);

  m_trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  m_trigger->AttachTo(Entrance);
  m_trigger->bHiddenInGame = false;
  m_trigger->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
  m_trigger->bGenerateOverlapEvents = true;
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
	
  RegisterDelegate();
}

void ATower::RegisterDelegate() {
  if (!m_trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap)) {
    m_trigger->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
}

void ATower::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    FVector dif = OtherActor->GetActorUpVector() - Entrance->GetUpVector();
    dif.X = (dif.X < 0) ? -dif.X : dif.X;
    dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
    dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
    if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05){
      AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
      if (gameMode)
        gameMode->EndGame(AMyGameMode::VICTORY);
    }
  }
}

void ATower::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (m_trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap))  {
    m_trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}