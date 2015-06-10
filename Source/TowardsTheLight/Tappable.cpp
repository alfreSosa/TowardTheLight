// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tappable.h"
#include "TimeManager.h"

// Sets default values
ATappable::ATappable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  //visble component
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  MeshActivator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshActivator"));
  MeshActivator->AttachTo(RootComponent);
  //trigger component
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;

  /* XXXComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
     XXXComponent->SetCollisionResponseToChannel(ECC_XXX, ECollisionResponse::ECR_Overlap);*/
  //default values public properties

  //initialize custom variables
  m_isPlayerOn = false;
}

// Called when the game starts or when spawned
void ATappable::BeginPlay()
{
	Super::BeginPlay();
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
  if (m_isPlayerOn) {
    Execute();
    if (GEngine)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH MECHANISM")));
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

