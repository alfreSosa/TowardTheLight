// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Stick.h"


AStick::AStick()
{
	PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  Stick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  Stick->SetMobility(EComponentMobility::Movable);
  Stick->AttachTo(RootComponent);
  UStaticMesh *mesh;
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/Models/Baculo/baculo_model.baculo_model'"));
  if (MeshFinder.Succeeded())
  {
    mesh = MeshFinder.Object;
    Stick->SetStaticMesh(mesh);
  }
  Stick->SetWorldScale3D(FVector(0.75, 0.75, 0.75));
  StickMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
  UMaterial* mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Models/Baculo/baculo_diffuse.baculo_diffuse'"));
  if (MatFinder.Succeeded())
  {
    mat = MatFinder.Object;
    StickMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }
}

void AStick::BeginPlay()
{
	Super::BeginPlay();
  Stick->SetMaterial(0, StickMaterial);
}

void AStick::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AStick::SetColor(FLinearColor colorKey, float shinnes) {
  StickMaterial->SetScalarParameterValue("Brillo", shinnes);
  StickMaterial->SetVectorParameterValue("BaculoColor", colorKey);
}
