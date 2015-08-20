// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Stick.h"


AStick::AStick(){
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  Stick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  Stick->SetMobility(EComponentMobility::Movable);
  Stick->CastShadow = false;
  Stick->AttachTo(RootComponent);
  UStaticMesh *mesh = nullptr;
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("StaticMesh'/Game/Models/Baculo/baculo_model.baculo_model'"));
  if (MeshFinder.Succeeded()){
    mesh = MeshFinder.Object;
    Stick->SetStaticMesh(mesh);
  }
  Stick->SetWorldScale3D(FVector(0.75, 0.75, 0.75));
  StickMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
  UMaterial* mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Models/Baculo/baculo_diffuse.baculo_diffuse'"));
  if (MatFinder.Succeeded()){
    mat = MatFinder.Object;
    StickMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }

  EffectsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshEffects"));
  EffectsMesh->AttachTo(RootComponent);
  EffectsMesh->SetMobility(EComponentMobility::Movable);
  EffectsMesh->CastShadow = false;
  mesh = nullptr;
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinderEffects(TEXT("StaticMesh'/Game/Models/Baculo/baculoBloom_mesh.baculoBloom_mesh'"));
  if (MeshFinderEffects.Succeeded()){
    mesh = MeshFinderEffects.Object;
    EffectsMesh->SetStaticMesh(mesh);
  }
  EffectsMesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 90)));
  EffectsMesh->SetRelativeLocation(FVector(0, 0, 70));
  EffectsMesh->SetWorldScale3D(FVector(0.75, 0.75, 0.75));
  EffectsMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(1);
  mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinderEffects(TEXT("Material'/Game/Models/Baculo/baculoBloom_material.baculoBloom_material'"));
  if (MatFinderEffects.Succeeded()){
    mat = MatFinderEffects.Object;
    EffectsMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }
}

void AStick::BeginPlay(){
	Super::BeginPlay();
  Stick->SetMaterial(0, StickMaterial);
  EffectsMesh->SetMaterial(0, EffectsMaterial);
}

void AStick::SetColor(FLinearColor colorKey, float shinnes) {
  StickMaterial->SetScalarParameterValue("Brillo", shinnes);
  StickMaterial->SetVectorParameterValue("BaculoColor", colorKey);
  EffectsMaterial->SetVectorParameterValue("Bloom_Color", colorKey);
}
