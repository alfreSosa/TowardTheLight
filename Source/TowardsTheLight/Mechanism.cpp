// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Mechanism.h"
#include "MobilePlatform.h"
#include "TimeManager.h"

AMechanism::AMechanism()
{
	PrimaryActorTick.bCanEverTick = true;
  //visble component
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  MeshActivator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshActivator"));
  MeshActivator->AttachTo(RootComponent);
  //trigger component
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("NoCollision")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
  //default values public properties
  Target = nullptr;
  //initialize custom variables
  m_isEnabled = false;
  m_isPlayerOn = false;
  m_mobileTarget = nullptr;
}

void AMechanism::BeginPlay()
{
	Super::BeginPlay();
  m_mobileTarget = dynamic_cast<AMobilePlatform *>(Target);
  //prueba de concepto
  if (m_mobileTarget) {
    m_isEnabled = true;
    if (GEngine)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("He asignado una movil como referencia")));
  }

  RegisterDelegate();
}

void AMechanism::Tick(float DeltaSeconds)
{
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);

  Super::Tick(DeltaSeconds);

}

//void AMechanism::OnActorBeginOverlap() {
//  if (OtherActor->ActorHasTag("Player")){
//    if (GEngine)
//      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OVERLAP")));
//    m_isPlayerOn = true;
//  }
//}

void AMechanism::RegisterDelegate() {
 /* if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AMechanism::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMechanism::OnBeginTriggerOverlap);
  }

  if (!Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &AMechanism::OnTriggerOverlapEnd)) {
    Trigger->OnComponentEndOverlap.AddDynamic(this, &AMechanism::OnTriggerOverlapEnd);
  }*/
}

void AMechanism::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    m_isPlayerOn = true;
  }
}

void AMechanism::OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player")){
    m_isPlayerOn = false;
  }
}

void  AMechanism::ReceiveActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex) 
{
  if (m_isEnabled)
    if (m_isPlayerOn)
      if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH MECHANISM")));
    
}

void AMechanism::EndPlay(const EEndPlayReason::Type EndPlayReason){
 /* if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AMechanism::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &AMechanism::OnBeginTriggerOverlap);
  }

  if (Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &AMechanism::OnTriggerOverlapEnd))  {
    Trigger->OnComponentEndOverlap.RemoveDynamic(this, &AMechanism::OnTriggerOverlapEnd);
  }*/
  Super::EndPlay(EndPlayReason);
}

