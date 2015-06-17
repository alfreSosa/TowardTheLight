// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "MobileEnemy.h"
#include "PlayerOvi.h"
#include "TimeManager.h"

AMobileEnemy::AMobileEnemy() {
  PrimaryActorTick.bCanEverTick = true;
  this->SetActorEnableCollision(true);

  //setting
  RightDistance = 100.f;
  LeftDistance = 100.f;
  RightDelay = 1.f;
  LeftDelay = 1.f;
  Speed = 100.f;
  InitialDelay = 1.f;
  HasTrigger = false;

  //private variables
  m_timer = 0;
  m_state = INITIAL_DELAY;
  m_currentDistance = 0;
  m_initMovement = false;
  m_jumpSpeed = 0.0f;
  m_actualJumpSpeed = m_accelerationJump = 0.0f;
  m_enableGravity = false;
  m_player = nullptr;

  //Capsule
  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  CapsuleComponent->InitCapsuleSize(DEFAULT_ENEMY_CAPSULE_RADIOUS, DEFAULT_ENEMY_CAPSULE_HEIGHT);

  static FName CollisionProfileName(TEXT("NoCollision"));
  CapsuleComponent->SetCollisionProfileName(CollisionProfileName);

  CapsuleComponent->CanCharacterStepUpOn = ECB_No;
  CapsuleComponent->bShouldUpdatePhysicsVolume = true;
  CapsuleComponent->bCheckAsyncSceneOnMove = false;
  CapsuleComponent->bCanEverAffectNavigation = false;
  CapsuleComponent->bGenerateOverlapEvents = true;
  //Fijamos por defecto la rotacion de la capsula para que el forward este de cara a la camara
  CapsuleComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 0)));
  CapsuleComponent->SetCapsuleHalfHeight(DEFAULT_ENEMY_CAPSULE_HEIGHT);
  //RootComponent = CapsuleComponent;

  //trigger
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
}


void AMobileEnemy::BeginPlay() {
  Super::BeginPlay();

  m_totalDistance = RightDistance;
  RegisterDelegate();
  m_actualJumpSpeed = 0.0f;
  m_enableGravity = false;
  m_capsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
  m_capsuleRadious = CapsuleComponent->GetScaledCapsuleRadius();
}

void AMobileEnemy::Tick(float DeltaSeconds) {
  DeltaSeconds = TimeManager::Instance()->GetDeltaTime(DeltaSeconds);
  m_lastPosition = GetActorLocation();
  if (!m_player) {
    for (TActorIterator< APawn > ActorItr(GetWorld()); ActorItr; ++ActorItr) {
      if (ActorItr->ActorHasTag("Player")) {
        m_player = (APlayerOvi*)*ActorItr;
        m_jumpSpeed = m_player->JumpSpeed;
        m_accelerationJump = m_player->AccelerationJump;
        break;
      }
    }
  }
  CheckCollision();
  CalculateGravity(DeltaSeconds);
  if (!HasTrigger || (HasTrigger && m_initMovement))
    doMovement(DeltaSeconds);
}

void AMobileEnemy::doMovement(float DeltaSeconds){
  switch (m_state){
  case INITIAL_DELAY:
    if (m_timer < InitialDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_RIGHT;
      m_rightVector = GetActorRightVector();
    }
    break;
  case TO_RIGHT:{
    float dist = Speed * DeltaSeconds;
    if (m_totalDistance - m_currentDistance < dist)
      dist = m_totalDistance - m_currentDistance;

    if (m_currentDistance < m_totalDistance){
      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += dist * m_rightVector;
      SetActorLocation(location);
    }
    else{
      m_state = RIGHT_DELAY;
      m_currentDistance = 0;
      if (m_totalDistance == RightDistance)
        m_totalDistance += LeftDistance;
    }
  }
    break;
  case TO_LEFT:{
    float dist = Speed * DeltaSeconds;
    if (m_totalDistance - m_currentDistance < dist)
      dist = m_totalDistance - m_currentDistance;

    if (m_currentDistance < m_totalDistance){
      m_currentDistance += dist;
      FVector location = GetActorLocation();
      location += dist * -m_rightVector;
      SetActorLocation(location);
    }
    else{
      m_state = LEFT_DELAY;
      m_currentDistance = 0;
    }
  }
    break;
  case RIGHT_DELAY:
    if (m_timer < RightDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_LEFT;
      FTransform transform = GetTransform();
      FQuat quat = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 180));
      transform.SetRotation(quat);
      SetActorTransform(transform);
    }
    break;
  case LEFT_DELAY:
    if (m_timer < LeftDelay)
      m_timer += DeltaSeconds;
    else{
      m_timer = 0;
      m_state = TO_RIGHT;
      FTransform transform = GetTransform();
      FQuat quat = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 180));
      transform.SetRotation(quat);
      SetActorTransform(transform);
    }
    break;
  }
}

void AMobileEnemy::RegisterDelegate() {
  if (!Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AMobileEnemy::OnBeginTriggerOverlap)) {
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMobileEnemy::OnBeginTriggerOverlap);
  }
}

void AMobileEnemy::OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
  if (OtherActor->ActorHasTag("Player")){
    m_initMovement = true;
  }
}

void AMobileEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason){
  if (Trigger->OnComponentBeginOverlap.IsAlreadyBound(this, &AMobileEnemy::OnBeginTriggerOverlap))  {
    Trigger->OnComponentBeginOverlap.RemoveDynamic(this, &AMobileEnemy::OnBeginTriggerOverlap);
  }
  Super::EndPlay(EndPlayReason);
}

void AMobileEnemy::CalculateGravity(float DeltaSeconds){
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();
  if (m_enableGravity) {
    if (m_actualJumpSpeed < m_jumpSpeed)
      m_actualJumpSpeed += m_accelerationJump * DeltaSeconds;
    else{
      m_actualJumpSpeed = m_jumpSpeed;
    }
    location -= m_actualJumpSpeed * DeltaSeconds * up;
    SetActorLocation(location);
  }
}

void AMobileEnemy::CheckCollision() {
  TArray<FHitResult> OutTraceResultDown;
  TArray<FHitResult> OutTraceResultMiddle;

  FVector StartTrace = m_lastPosition;

  FVector newLocationForward;
  newLocationForward.X = (FMath::Abs(GetActorForwardVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationForward.Y = (FMath::Abs(GetActorForwardVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationForward.Z = (FMath::Abs(GetActorForwardVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  const FVector EndTraceMidle = newLocationForward + GetActorForwardVector() * m_capsuleRadious;

  static FName FireTraceIdent = FName(TEXT("Platform"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bFindInitialOverlaps = true;
  TraceParams.bTraceComplex = true;

  FCollisionResponseParams ResponseParam(ECollisionResponse::ECR_Overlap);

  GetWorld()->LineTraceMulti(OutTraceResultMiddle, StartTrace, EndTraceMidle, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionMidle = OutTraceResultMiddle.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceMidle, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  if (collisionMidle) {
    int size = OutTraceResultMiddle.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultMiddle[i].GetActor()->ActorHasTag("Platform")) {
        //SetActorLocation(RecalculateLocation(GetActorForwardVector(), GetActorLocation(), OutTraceResultMiddle[i].Location, m_capsuleRadious));
        //aqui hacer que cambie de direccion.
        break;
      }
  }

  FVector newLocationUp;
  newLocationUp.X = (FMath::Abs(GetActorUpVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationUp.Y = (FMath::Abs(GetActorUpVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationUp.Z = (FMath::Abs(GetActorUpVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  const FVector EndTraceDown = newLocationUp - GetActorUpVector() * m_capsuleHeight;

  GetWorld()->LineTraceMulti(OutTraceResultDown, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDown = OutTraceResultDown.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceDown, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

    if (collisionDown) {
      int size = OutTraceResultDown.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDown[i].GetActor()->ActorHasTag("Platform")) {
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDown[i].Location, -m_capsuleHeight));
          m_enableGravity = false;
          m_actualJumpSpeed = m_jumpSpeed;
          break;
        }
    }
    else {
      m_enableGravity = true;
    }
  }

FVector AMobileEnemy::AbsVector(const FVector& vector) {
  FVector absVector = FVector::ZeroVector;

  absVector.X = (vector.X < 0) ? -vector.X : vector.X;
  absVector.Y = (vector.Y < 0) ? -vector.Y : vector.Y;
  absVector.Z = (vector.Z < 0) ? -vector.Z : vector.Z;

  return absVector;
}

FVector AMobileEnemy::RecalculateLocation(FVector Direction, FVector Location, FVector HitLocation, float size)
{
  FVector loc = Location;
  FVector absDirection = FVector::ZeroVector;
  FVector dir = Direction;

  absDirection.X = (FMath::Abs(dir.X) <= 0.01) ? 0 : 1;
  absDirection.Y = (FMath::Abs(dir.Y) <= 0.01) ? 0 : 1;
  absDirection.Z = (FMath::Abs(dir.Z) <= 0.01) ? 0 : 1;

  FVector a = (absDirection * HitLocation) - (Direction * size);
  FVector nPos = (absDirection * a);

  loc.X = (FMath::Abs(nPos.X) <= 0.01) ? loc.X : nPos.X;
  loc.Y = (FMath::Abs(nPos.Y) <= 0.01) ? loc.Y : nPos.Y;
  loc.Z = (FMath::Abs(nPos.Z) <= 0.01) ? loc.Z : nPos.Z;

  return loc;
}