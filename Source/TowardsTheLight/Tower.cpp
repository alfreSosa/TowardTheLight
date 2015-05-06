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

  m_trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  m_trigger->AttachTo(Entrance);
  m_trigger->bHiddenInGame = true;
  //m_trigger->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
  m_trigger->bGenerateOverlapEvents = true;
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
	
  RegisterDelegate();
}

// Called every frame
void ATower::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

}

void ATower::RegisterDelegate() {
  if (!m_trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap)) {
    m_trigger->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
}

void ATower::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    if (OtherActor->GetActorUpVector() == Entrance->GetUpVector()){
      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("VICTORIA!!!!")));
      }
    }
  }
}

void ATower::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (m_trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap))  {
    m_trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}