// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "StaticEnemy.h"
#include "TowardsTheLightGameMode.h"
#include "PlayerOvi.h"
#include "TimeManager.h"

// Sets default values
AStaticEnemy::AStaticEnemy() {
  PrimaryActorTick.bCanEverTick = true;
  //public properties
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  OurVisibleComponent->SetMobility(EComponentMobility::Movable);
  OurVisibleComponent->AttachTo(RootComponent);
  OurVisibleComponent->bGenerateOverlapEvents = true;
  OurVisibleComponent->CastShadow = false;
  this->SetActorEnableCollision(true);
  
  TimeToKill = 0.25f;

  //private properties
  m_touchingPlayer = false;
  m_elapsedTime = 0.0f;
}

// Called when the game starts or when spawned
void AStaticEnemy::BeginPlay() {
	Super::BeginPlay();
  m_touchingPlayer = false;
  m_elapsedTime = 0.0f;
  RegisterDelegate();
}

void AStaticEnemy::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  if (m_touchingPlayer) {
    m_elapsedTime += DeltaSeconds;
    if (m_elapsedTime >= TimeToKill) {
      ATowardsTheLightGameMode *gameMode = Cast<ATowardsTheLightGameMode>(UGameplayStatics::GetGameMode(this));
      if (gameMode)
        if (gameMode->EndGameBP() > -0.05)
          gameMode->EndGame(ATowardsTheLightGameMode::DEFEAT);
    }
  }
}
void AStaticEnemy::RegisterDelegate() {

  if (!OurVisibleComponent->OnComponentBeginOverlap.IsBound()) {
    m_delegate.BindUFunction(this, TEXT("OnCollision"));
    OurVisibleComponent->OnComponentBeginOverlap.Add(m_delegate);
  }
  if (!OurVisibleComponent->OnComponentEndOverlap.IsBound()) {
    m_delegateEnd.BindUFunction(this, TEXT("OnEndCollision"));
    OurVisibleComponent->OnComponentEndOverlap.Add(m_delegateEnd);
  }
}

void AStaticEnemy::OnCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player")){
    m_touchingPlayer = true;
    m_elapsedTime = 0.0f;
  }
}

void AStaticEnemy::OnEndCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player")){
    m_touchingPlayer = false;
    m_elapsedTime = 0.0f;
  }
}

void AStaticEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (OurVisibleComponent->OnComponentBeginOverlap.IsBound())
    OurVisibleComponent->OnComponentBeginOverlap.Remove(m_delegate);

  if (OurVisibleComponent->OnComponentEndOverlap.IsBound())
    OurVisibleComponent->OnComponentEndOverlap.Remove(m_delegateEnd);
  Super::EndPlay(EndPlayReason);
}

void AStaticEnemy::RestoreInitialState() {
  m_touchingPlayer = false;
  m_elapsedTime = 0.0f;
}