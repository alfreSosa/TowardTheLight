// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Altar.h"
#include "PlayerOvi.h"

// Sets default values
AAltar::AAltar()
{
	//PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);
  GiveKey = true;
  AltarColor = FLinearColor(0.0f, 0.0f, 1.0f);

  AltarMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
  UMaterial* mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Models/Altar/Altar_mat.Altar_mat'"));
  if (MatFinder.Succeeded())
  {
    mat = MatFinder.Object;
    AltarMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }
}

// Called when the game starts or when spawned
void AAltar::BeginPlay(){
	Super::BeginPlay();
  m_meshActivator->SetMaterial(0, AltarMaterial);
  AltarMaterial->SetVectorParameterValue("Color", AltarColor);
  MaterialBB->SetVectorParameterValue("Bloom_Color", AltarColor);
}

void AAltar::Activate(bool enabled) {

}

void AAltar::Execute() {
  m_player->SetKey(GiveKey, AltarColor);
  m_player->EnabledPickAltar();
}

