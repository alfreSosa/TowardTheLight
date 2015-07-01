// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Mechanism.h"
#include "StaticPlatform.h"
#include "TimeManager.h"

AMechanism::AMechanism()
{
	PrimaryActorTick.bCanEverTick = true;
  //Public properties
  CanActivate = CanDisactivate = true;
  DisableAtEndAction = false;
  NumberOfActions = 1;
  TimeInIntermittence = 1.0f;
  TimeToStartIntermittence = 5.0f;
  ColorDisabled = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
  ColorEnabled = FLinearColor(0.0f, 0.9490f, 1.0f, 1.0f);
  TargetsAreEnabled = false;
  intermitedOn = true;
  m_target = ColorEnabled;
  m_origin = ColorDisabled;
  //Private properties
  m_elapsedIntermitence = 0.0f;
  m_elapsedStartIntermitence = TimeToStartIntermittence;

  MechanismMaterial = ((UPrimitiveComponent*)GetRootComponent())->CreateAndSetMaterialInstanceDynamic(0);
  UMaterial* mat = nullptr;
  static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/Models/Button/Button.Button'"));
  if (MatFinder.Succeeded())
  {
    mat = MatFinder.Object;
    MechanismMaterial = UMaterialInstanceDynamic::Create(mat, GetWorld());
  }
}

void AMechanism::BeginPlay()
{
	Super::BeginPlay();
  m_meshActivator->SetMaterial(0, MechanismMaterial);
  int32 numTargets = Targets.Num();
  m_Targets.Init(numTargets);
  for (int32 i = 0; i < numTargets; i++)
    m_Targets[i] = dynamic_cast<AStaticPlatform *>(Targets[i]);

  for (int32 i = 0; i < numTargets; i++)
    m_Targets[i]->InitByMechanism(DisableAtEndAction, NumberOfActions);

  m_target = (intermitedOn) ? ColorEnabled : ColorDisabled;
  m_origin = (!intermitedOn) ? ColorEnabled : ColorDisabled;
}

void AMechanism::Tick(float DeltaSeconds)
{
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  Super::Tick(DeltaSeconds);
  m_elapsedStartIntermitence += DeltaSeconds;
  if (m_isPlayerOn) {
    if (m_elapsedStartIntermitence >= TimeToStartIntermittence) {
      m_elapsedIntermitence += DeltaSeconds;
      float t = (m_elapsedIntermitence / TimeInIntermittence);
      t = (t > 1.0f) ? 1.0f : t;
      FLinearColor actual = FMath::Lerp(m_origin, m_target, t);
      MechanismMaterial->SetVectorParameterValue("Color", actual);
      if (m_elapsedIntermitence >= TimeInIntermittence) {
        m_elapsedIntermitence = 0.0f;
        intermitedOn = !intermitedOn;
        m_target = (intermitedOn) ? ColorEnabled : ColorDisabled;
        m_origin = (!intermitedOn) ? ColorEnabled : ColorDisabled;
      }
    }
  }
  else {
    m_elapsedStartIntermitence = TimeToStartIntermittence;
    int32 numTargets = m_Targets.Num();
    bool some = false;
    for (int32 i = 0; i < numTargets; i++) 
      if (m_Targets[i]->isEnabled()) {
        some = true;
        break;
      }
    FLinearColor actual = (some) ? ColorEnabled : ColorDisabled;
    m_target = (intermitedOn) ? ColorEnabled : ColorDisabled;
    m_origin = (!intermitedOn) ? ColorEnabled : ColorDisabled;
    MechanismMaterial->SetVectorParameterValue("Color", actual);
  }
}

void AMechanism::Activate(bool enabled) {

}

void AMechanism::Execute() {
  int32 numTargets = m_Targets.Num();
  for (int32 i = 0; i < numTargets; i++) {
    if (m_Targets[i]->isEnabled()) {
      if (CanDisactivate) {
        m_elapsedStartIntermitence = 0.0f;
        MechanismMaterial->SetVectorParameterValue("Color", ColorDisabled);
        m_target = ColorEnabled;
        m_origin = ColorDisabled;
        m_Targets[i]->ChangeEnabled(false);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Platform Desactivated")));
      }
    }
    else {
      if (CanActivate) {
        m_elapsedStartIntermitence = 0.0f;
        MechanismMaterial->SetVectorParameterValue("Color", ColorEnabled);
        m_target = ColorDisabled;
        m_origin = ColorEnabled;
        m_Targets[i]->ChangeEnabled(true);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Platform Activated")));
      }
    }
  }
}


