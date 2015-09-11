// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tower.h"
#include "TowardsTheLightGameMode.h"
#include "PlayerOvi.h"
#include "TimeManager.h"
#include "InfoGameInstance.h"

// Sets default values
ATower::ATower() {
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
//  Body->SetWorldScale3D(FVector(2.5, 2.5, 2.5));
  RootComponent->SetMobility(EComponentMobility::Static);
  Body->SetMobility(EComponentMobility::Static);
  Body->CastShadow = false;
  Body->AttachTo(RootComponent);
  this->Tags.Add("Platform");

  Light = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
  Light->SetMobility(EComponentMobility::Static);
  Light->CastShadow = false;
  Light->AttachTo(Body);

  Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
  Entrance->SetMobility(EComponentMobility::Static);
  Entrance->CastShadow = false;
  Entrance->AttachTo(Body);

  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->AttachTo(Entrance);
  Trigger->bHiddenInGame = true;
  Trigger->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
  Trigger->bGenerateOverlapEvents = true;

  ColorDisabled = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
  ColorEnabled = FLinearColor(0.0f, 0.9490f, 1.0f, 1.0f);

  NeedKey = false;
  ColorKey = FLinearColor(0.0f, 0.0f, 0.0f);

  TowerLightMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
  UMaterial* mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Models/Tower/Tower_sphere_material.Tower_sphere_material'"));
  if (MatFinder.Succeeded())
  {
    mat = MatFinder.Object;
    TowerLightMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }

  TowerRunesMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(1);
  UMaterial* mat2 = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatRunesFinder(TEXT("Material'/Game/Models/Tower/torre.torre'"));
  if (MatRunesFinder.Succeeded())
  {
    mat2 = MatRunesFinder.Object;
    TowerRunesMaterial = UMaterialInstanceDynamic::Create(mat2, GetWorld());
  }

  MaterialBB = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(2);
  mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinderEffectsBB(TEXT("Material'/Game/Models/Baculo/baculoBloom_material.baculoBloom_material'"));
  if (MatFinderEffectsBB.Succeeded()){
    mat = MatFinderEffectsBB.Object;
    MaterialBB = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }

  EffectsBB = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("BB"));
  EffectsBB->AttachTo(RootComponent);
  EffectsBB->CastShadow = false;

  m_startVictory = false;
  m_timeToFinish = 2.0f;
  m_elapsedTime = 0.0f;

}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
  Light->SetMaterial(0, TowerLightMaterial);
  Body->SetMaterial(0, TowerRunesMaterial);
  TowerLightMaterial->SetVectorParameterValue("Color", ColorDisabled);
  TowerRunesMaterial->SetVectorParameterValue("ColorRunes", ColorKey);
  RegisterDelegate();
  m_startVictory = false;
  m_timeToFinish = 2.0f;
  m_elapsedTime = 0.0f;

  EffectsBB->AddElement(MaterialBB, NULL, false, 100, 100, NULL);
  if (NeedKey)
    MaterialBB->SetVectorParameterValue("Bloom_Color", ColorKey);
  else
    MaterialBB->SetVectorParameterValue("Bloom_Color", FLinearColor(FVector(0.f)));

  Cast<UInfoGameInstance>(GetGameInstance())->SetTowerNeedKey(NeedKey);
  Cast<UInfoGameInstance>(GetGameInstance())->SetTowerKeyColor(ColorKey);
}

void ATower::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  Super::Tick(DeltaSeconds);
  
  if (m_startVictory) {
    float t = m_elapsedTime / m_timeToFinish;
    t = (t > 1.0f) ? 1.0f : t;
    FLinearColor color = FMath::Lerp(ColorDisabled, ColorEnabled, t);
    TowerLightMaterial->SetVectorParameterValue("Color", color);
    m_elapsedTime += DeltaSeconds;
  }
}

void ATower::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
}

void ATower::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    APlayerOvi *player = dynamic_cast<APlayerOvi *>(OtherActor);
    if (player){
      FVector dif = player->GetActorUpVector() - Entrance->GetUpVector();
      dif.X = (dif.X < 0) ? -dif.X : dif.X;
      dif.Y = (dif.Y < 0) ? -dif.Y : dif.Y;
      dif.Z = (dif.Z < 0) ? -dif.Z : dif.Z;
      if (dif.X < 0.05 && dif.Y < 0.05 && dif.Z < 0.05){
        if (!NeedKey || (NeedKey && player->HasKey() && ColorKey == player->GetColorKey())){
          ATowardsTheLightGameMode *gameMode = Cast<ATowardsTheLightGameMode>(UGameplayStatics::GetGameMode(this));
          if (gameMode) {
            m_startVictory = true;
            gameMode->EndGame(ATowardsTheLightGameMode::VICTORY);
          }
        }
      }
    }
  }
}

void ATower::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &ATower::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATower::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}

void ATower::RestoreInitialState() {
  m_startVictory = false;
  TowerLightMaterial->SetVectorParameterValue("Color", ColorDisabled);
  m_timeToFinish = 2.0f;
  m_elapsedTime = 0.0f;
}
