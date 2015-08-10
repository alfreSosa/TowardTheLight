// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "PickableItem.h"
#include "TimeManager.h"
#include "MyGameMode.h"

// Sets default values
APickableItem::APickableItem() {
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->CastShadow = false;
  OurVisibleComponent->AttachTo(RootComponent);
  
  this->SetActorEnableCollision(true);

  Points = DEFAULT_POINTS;
  m_collected = false;
  m_initialPosition = FVector::ZeroVector;
}

void APickableItem::BeginPlay() 
{
  m_initialPosition = GetActorLocation();
}


void APickableItem::Tick(float DeltaSeconds){
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);

  if (IsOrb){
    FRotator MyRot = GetActorRotation();
    MyRot.Yaw += 50 * DeltaSeconds;
    MyRot.Roll += 75 * DeltaSeconds;
    SetActorRotation(MyRot);
  }
}

void APickableItem::ReceiveActorBeginOverlap(AActor* OtherActor) {
  if (OtherActor->ActorHasTag("Player") && !m_collected) {
    m_collected = true;  //auqnue el ojeto se destruya, es bueno dejarlo por si al frame siguiente la memoria no se ha liberado aún
    this->SetActorEnableCollision(false);
    SetActorLocation(FVector(0, 0, 0));
    PrimaryActorTick.bCanEverTick = false; //para que deje de actualizarse, si se recupera en un checkpoint, activar
    AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
    if (gameMode) {
      gameMode->AddPoints(this->Points);
      if (IsOrb)
        gameMode->OrbPicked();
    }
    //this->Destroy();
  }
}

bool APickableItem::IsItemPicked() {
  return m_collected;
}

void APickableItem::RestoreInitialPosition() {
  this->SetActorEnableCollision(true);
  PrimaryActorTick.bCanEverTick = true;
  SetActorLocation(m_initialPosition);
  m_collected = false;
}



