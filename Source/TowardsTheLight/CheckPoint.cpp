#include "TowardsTheLight.h"
#include "CheckPoint.h"
#include "MyGameMode.h"
#include "PickableItem.h"
#include "PlayerOvi.h"

ACheckPoint::ACheckPoint()
{
  //PrimaryActorTick.bCanEverTick = true;
  //initialize public properties
  //trigger component
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  CheckPointVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckPointVisibleComponent"));
  CheckPointVisibleComponent->AttachTo(RootComponent);
  CheckPointVisibleComponent->bGenerateOverlapEvents = true;

  //tendra material dinamico

  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
  Trigger->AttachTo(RootComponent);
  //initialize private properties
  m_loaded = false;
  m_enter = false;
}

void ACheckPoint::BeginPlay()
{
  Super::BeginPlay();
  RegisterDelegate();
  m_loaded = false;
  m_enter = false;
  m_gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
}

void ACheckPoint::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

void ACheckPoint::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ACheckPoint::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnBeginTriggerOverlap);
  }

  if (!Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ACheckPoint::OnTriggerOverlapEnd)) {
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnTriggerOverlapEnd);
  }
}

void ACheckPoint::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded && !m_enter) {
    APlayerOvi *player = dynamic_cast<APlayerOvi *>(OtherActor);
    if (player) {
      m_enter = true;
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GET CHECKPOINT")));
      //rellenar checkPoint MyGameMode
      
      m_gameMode->SetPlayerCheckPoint(player, player->GetTransform(), player->PlayerisToRight());
      m_gameMode->SetPlayerKey(player->HasKey(), player->GetColorKey());
      for (TActorIterator<APickableItem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        if (ActorItr->IsItemPicked())
          m_gameMode->AddItemPicked(*ActorItr);
    }
  }

}

void ACheckPoint::OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
  if (OtherActor->ActorHasTag("Player") && !m_loaded) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SAliendo checkpoint")));
    m_loaded = true;
  }
}

void ACheckPoint::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ACheckPoint::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ACheckPoint::OnBeginTriggerOverlap);
  }

  if (Trigger->OnComponentEndOverlap.IsAlreadyBound(this, &ACheckPoint::OnTriggerOverlapEnd))  {
    Trigger->OnComponentEndOverlap.RemoveDynamic(this, &ACheckPoint::OnTriggerOverlapEnd);
  }
  Super::EndPlay(EndPlayReason);
}



