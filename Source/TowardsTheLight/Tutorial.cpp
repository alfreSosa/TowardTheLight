#include "TowardsTheLight.h"
#include "Tutorial.h"
#include "LocalizationManager.h"

ATutorial::ATutorial()
{
	//PrimaryActorTick.bCanEverTick = true;
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
  Localizador = "Default";
}

void ATutorial::BeginPlay()
{
	Super::BeginPlay();
  RegisterDelegate();
  m_loaded = false;
  m_enter = false;
  m_text = LocalizationManager::Instance()->GetString(Localizador);
}

void ATutorial::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATutorial::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATutorial::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATutorial::OnBeginTriggerOverlap);
  }

  if (!Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ATutorial::OnTriggerOverlapEnd)) {
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ATutorial::OnTriggerOverlapEnd);
  }
}

void ATutorial::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded && !m_enter) {
    m_enter = true;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, m_text);
  }
  
}

void ATutorial::OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SALIENDO TUTORIAAAAAL")));
    m_loaded = true;
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

