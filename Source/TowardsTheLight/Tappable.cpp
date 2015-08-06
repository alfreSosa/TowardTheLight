// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tappable.h"
#include "TimeManager.h"
#include "PlayerOvi.h"

// Sets default values
ATappable::ATappable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  //visble component
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  MeshActivator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshActivator"));
  MeshActivator->AttachTo(RootComponent);
  RootComponent->SetMobility(EComponentMobility::Static);
  MeshActivator->SetMobility(EComponentMobility::Static);
  MeshActivator->CastShadow = false;
  m_meshActivator = MeshActivator;
  //trigger component
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;

  //initialize custom variables
  m_isPlayerOn = false;
  m_player = nullptr;
  //initialize custom editor variables
  NeedKey = false;
  ColorKey = FLinearColor(0.0f, 0.0f, 0.0f);
  this->Tags.Add("Tappable");
}

// Called when the game starts or when spawned
void ATappable::BeginPlay()
{
	Super::BeginPlay();
  m_meshActivator = MeshActivator;
  RegisterDelegate();
}

// Called every frame
void ATappable::Tick( float DeltaTime )
{
  DeltaTime = TimeManager::Instance()->GetDeltaTime(DeltaTime);
	Super::Tick( DeltaTime );

}

void ATappable::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATappable::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATappable::OnBeginTriggerOverlap);
  }

  if (!Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ATappable::OnTriggerOverlapEnd)) {
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ATappable::OnTriggerOverlapEnd);
  }
}

void ATappable::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    m_player = dynamic_cast<APlayerOvi *>(OtherActor);
    FVector dif = m_player->GetActorUpVector() - GetActorUpVector();
    dif.X = (dif.X < 0) ? -dif.X : dif.X;
    dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
    dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
    if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05)
      m_isPlayerOn = true;
    Activate(true);
  }
}

void ATappable::OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player")){
    m_isPlayerOn = false;
    Activate(false);
  }
}

void  ATappable::ReceiveActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex)
{
  if (m_isPlayerOn)
    if (m_player)
      if (!NeedKey || (NeedKey && m_player->HasKey() && ColorKey == m_player->GetColorKey())) {
          Execute();
      }
}

void ATappable::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATappable::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATappable::OnBeginTriggerOverlap);
  }

  if (Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ATappable::OnTriggerOverlapEnd))  {
    Trigger->OnComponentEndOverlap.RemoveDynamic(this, &ATappable::OnTriggerOverlapEnd);
  }
  Super::EndPlay(EndPlayReason);
}

void ATappable::Activate(bool enabled) {

}

void ATappable::Execute() {

}

