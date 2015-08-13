#include "TowardsTheLight.h"
#include "Tutorial.h"
#include "TowardsTheLightGameMode.h"


ATutorial::ATutorial() {
  //initialize public properties
  //trigger component
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
  RootComponent = Trigger;
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
}

void ATutorial::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATutorial::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATutorial::OnBeginTriggerOverlap);
  }

  if (!Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ATutorial::OnTriggerOverlapEnd)) {
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ATutorial::OnTriggerOverlapEnd);
  }
}

void ATutorial::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATutorial::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATutorial::OnBeginTriggerOverlap);
  }

  if (Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ATutorial::OnTriggerOverlapEnd))  {
    Trigger->OnComponentEndOverlap.RemoveDynamic(this, &ATutorial::OnTriggerOverlapEnd);
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