// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tower.h"
#include "MyGameMode.h"
#include "PlayerOvi.h"


// Sets default values
ATower::ATower() {
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
//  Body->SetWorldScale3D(FVector(2.5, 2.5, 2.5));
  RootComponent->SetMobility(EComponentMobility::Static);
  Body->SetMobility(EComponentMobility::Static);
  Body->AttachTo(RootComponent);
  this->Tags.Add("Platform");

  Light = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
  Light->SetMobility(EComponentMobility::Static);
  Light->AttachTo(Body);

  Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
  Entrance->SetMobility(EComponentMobility::Static);
  Entrance->AttachTo(Body);

  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->AttachTo(Entrance);
  Trigger->bHiddenInGame = true;
  Trigger->SetWorldScale3D(FVector(0.5, 0.5, 0.5));
  Trigger->bGenerateOverlapEvents = true;

  ColorDisabled = FColor(0.0f, 0.0f, 0.0f, 1.0f);
  ColorEnabled = FColor(0.0f, 0.9490f, 1.0f, 1.0f);

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
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();
  Light->SetMaterial(0, TowerLightMaterial);
  TowerLightMaterial->SetVectorParameterValue("Color", ColorDisabled);
  RegisterDelegate();
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
          AMyGameMode *gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this));
          if (gameMode) {
            TowerLightMaterial->SetVectorParameterValue("Color", ColorEnabled);
            gameMode->EndGame(AMyGameMode::VICTORY);
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