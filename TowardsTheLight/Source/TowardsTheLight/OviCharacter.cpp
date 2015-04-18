// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "OviCharacter.h"
#include <math.h>

AOviCharacter::AOviCharacter(){
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  AutoPossessPlayer = EAutoReceiveInput::Player0;

  //Habilito rotaciones sobre todos los ejes
  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  //yaw es sobre z
  bUseControllerRotationYaw = false;

  m_state = States::STOP;
  m_limit = 0;

 
}

// Called when the game starts or when spawned
void AOviCharacter::BeginPlay(){
  Super::BeginPlay();
  m_limit = FVector::DotProduct(GetActorLocation(), GetActorForwardVector());
  m_limit = abs(m_limit);
}

// Called every frame
void AOviCharacter::Tick(float DeltaTime){
  Super::Tick(DeltaTime);
  float value = 0;

  FRotator rot = GetActorRotation();
  if (m_right && !m_left){
    value = 1;
    if (m_state != States::RIGHT)
      rot.Yaw -= 90;
    m_state = States::RIGHT;
  }
  else if (m_left && !m_right){
    value = 1;
    if (m_state != States::LEFT)
      rot.Yaw += 90;
    m_state = States::LEFT;
  }
  else{
    value = 0;
    if (m_state == States::RIGHT)
      rot.Yaw += 90;
    if (m_state == States::LEFT)
      rot.Yaw -= 90;

    m_state = States::STOP;
  }
  SetActorRotation(rot);
  FVector forward = GetActorForwardVector();

  /*if (GEngine)
  {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f, z: %f "), forward.X, forward.Y, forward.Z));
  }*/

  AddMovementInput(FVector(value, value, value) * forward, 1);

  float dotForward = FVector::DotProduct(GetActorLocation(), forward);

  if (dotForward > m_limit && m_state == States::RIGHT)
    rot.Yaw -= 90;
  else if (dotForward > m_limit && m_state == States::LEFT)
    rot.Yaw += 90;

  SetActorRotation(rot);
}

// Called to bind functionality to input
void AOviCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
  Super::SetupPlayerInputComponent(InputComponent);
  InputComponent->BindAction("MoveRight", IE_Pressed, this, &AOviCharacter::OnStartRight);
  InputComponent->BindAction("MoveRight", IE_Released, this, &AOviCharacter::OnStopRight);
  InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AOviCharacter::OnStartLeft);
  InputComponent->BindAction("MoveLeft", IE_Released, this, &AOviCharacter::OnStopLeft);
}

void AOviCharacter::OnStartRight(){
  m_right = true;
}
void AOviCharacter::OnStopRight(){
  m_right = false;
}

void AOviCharacter::OnStartLeft(){
  m_left = true;
}
void AOviCharacter::OnStopLeft(){
  m_left = false;
}
