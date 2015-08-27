#include "TowardsTheLight.h"
#include "Tutorial.h"
#include "TowardsTheLightGameMode.h"


ATutorial::ATutorial() {
  //initialize public properties
  AutoLoad = false;
  //trigger component
  TriggerIn = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerIn"));
  TriggerIn->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  TriggerIn->bHiddenInGame = true;
  TriggerIn->bGenerateOverlapEvents = true;
  TriggerIn->AttachTo(RootComponent);

  TriggerOut = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerOut"));
  TriggerOut->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  TriggerOut->bHiddenInGame = true;
  TriggerOut->bGenerateOverlapEvents = true;
  TriggerOut->AttachTo(RootComponent);
  //initialize private properties
  m_loaded = false;
  m_enter = false;
}

void ATutorial::BeginPlay() {
	Super::BeginPlay();
  RegisterDelegate();

  //Get ATowardsTheLightGameMode
  m_gameMode = Cast<ATowardsTheLightGameMode>(UGameplayStatics::GetGameMode(this));

  m_loaded = false;
  m_enter = false;
  if (AutoLoad) {
    m_enter = true;
    m_gameMode->EnterTutorialEvent(Key);
  }
}

void ATutorial::RegisterDelegate() {
  if (!TriggerIn->OnComponentBeginOverlap.IsAlreadyBound(this, &ATutorial::OnBeginTriggerOverlap)) {
    TriggerIn->OnComponentBeginOverlap.AddDynamic(this, &ATutorial::OnBeginTriggerOverlap);
  }

  if (!TriggerOut->OnComponentEndOverlap.IsAlreadyBound(this, &ATutorial::OnTriggerOverlapEnd)) {
    TriggerOut->OnComponentEndOverlap.AddDynamic(this, &ATutorial::OnTriggerOverlapEnd);
  }
}

void ATutorial::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  if (TriggerIn->OnComponentBeginOverlap.IsAlreadyBound(this, &ATutorial::OnBeginTriggerOverlap))  {
    TriggerIn->OnComponentBeginOverlap.RemoveDynamic(this, &ATutorial::OnBeginTriggerOverlap);
  }

  if (TriggerOut->OnComponentEndOverlap.IsAlreadyBound(this, &ATutorial::OnTriggerOverlapEnd))  {
    TriggerOut->OnComponentEndOverlap.RemoveDynamic(this, &ATutorial::OnTriggerOverlapEnd);
  }
  Super::EndPlay(EndPlayReason);
}

void ATutorial::RestoreInitialState(){
  m_loaded = false;
  m_enter = false;
}

void ATutorial::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded && !m_enter) {
    m_enter = true;

    m_gameMode->EnterTutorialEvent(Key);
  }
}

void ATutorial::OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded) {
    m_loaded = true;

    m_gameMode->ExitTutorialEvent();
  }
}