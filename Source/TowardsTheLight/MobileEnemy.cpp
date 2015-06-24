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
  m_enableGravity = true;
  m_player = nullptr;
  //Capsule
  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  CapsuleComponent->InitCapsuleSize(DEFAULT_ENEMY_CAPSULE_RADIOUS, DEFAULT_ENEMY_CAPSULE_HEIGHT);

  static FName CollisionProfileName(TEXT("OverlapAll"));
  CapsuleComponent->SetCollisionProfileName(CollisionProfileName);

  CapsuleComponent->CanCharacterStepUpOn = ECB_No;
  CapsuleComponent->bShouldUpdatePhysicsVolume = true;
  CapsuleComponent->bCheckAsyncSceneOnMove = false;
  CapsuleComponent->bCanEverAffectNavigation = false;
  CapsuleComponent->bGenerateOverlapEvents = true;
  //Fijamos por defecto la rotacion de la capsula para que el forward este de cara a la camara
  CapsuleComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 0)));
  CapsuleComponent->SetCapsuleHalfHeight(DEFAULT_ENEMY_CAPSULE_HEIGHT);
  CapsuleComponent->AttachTo(RootComponent);
  CapsuleComponent->SetRelativeLocation(FVector(0, 0, 0));

  //trigger
  Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
  Trigger->SetCollisionProfileName(FName(TEXT("OverlapOnlyPawn")));
  Trigger->AttachTo(RootComponent);
  Trigger->bHiddenInGame = true;
  Trigger->bGenerateOverlapEvents = true;
  Trigger->SetRelativeLocation(FVector(0, 0, 0));
}


void AMobileEnemy::BeginPlay() {
  Super::BeginPlay();
  m_totalDistance = RightDistance + LeftDistance;
  m_currentDistance = LeftDistance;
  m_actualJumpSpeed = 0.0f;
  m_capsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
  m_capsuleRadious = CapsuleComponent->GetScaledCapsuleRadius();
  m_capsuleHeightPadding = m_capsuleHeight * PADDING_ENEMY_COLLISION_PERCENT;
  m_lastPosition = GetActorLocation();
  
  {
    FVector right = GetActorRightVector();
    right = right;
    m_rightPosition = (m_lastPosition + RightDistance * right) * right;
    m_leftPosition = (m_lastPosition + LeftDistance * -right) * right;
  }

  RegisterDelegate();

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
  if (!HasTrigger || (HasTrigger && m_initMovement)) {
    doMovement(DeltaSeconds);
    CalculateGravity(DeltaSeconds);
    CheckCollision();
  }
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
  else {
    m_actualJumpSpeed = 0;
  }
}

void AMobileEnemy::CheckCollision() {
  TArray<FHitResult> OutTraceResultDown;
  TArray<FHitResult> OutTraceResultDownLeftF;
  TArray<FHitResult> OutTraceResultDownRigthF;

  TArray<FHitResult> OutTraceResultTop;
  TArray<FHitResult> OutTraceResultMiddle;
  TArray<FHitResult> OutTraceResultBottom;

  TArray<FHitResult> OutTraceResultTopBack;
  TArray<FHitResult> OutTraceResultMiddleBack;
  TArray<FHitResult> OutTraceResultBottomBack;


  // Calculate the start location for trace  
  FVector StartTrace = m_lastPosition;
  FVector StartTraceTop = StartTrace + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding);
  FVector StartTraceBottom = StartTrace - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding);

  FVector StartTraceBody = StartTrace + GetActorUpVector() * m_capsuleHeight;
  FVector StartTraceLegs = StartTrace - GetActorUpVector() * m_capsuleHeight;

  FVector StartTraceLeftF = StartTrace + GetActorRightVector() * m_capsuleRadious;
  FVector StartTraceRigthF = StartTrace - GetActorRightVector() * m_capsuleRadious;
  // Calculate the end location for trace  
  //horizontal
  FVector newLocationForward;
  newLocationForward.X = (FMath::Abs(GetActorRightVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationForward.Y = (FMath::Abs(GetActorRightVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationForward.Z = (FMath::Abs(GetActorRightVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  // Calculate the start location for trace back
  FVector StartTraceBack = newLocationForward;
  FVector StartTraceTopBack = StartTraceBack + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding);
  FVector StartTraceBottomBack = StartTraceBack - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding);

  const FVector EndTraceTop = (newLocationForward + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) + GetActorRightVector() * m_capsuleRadious;
  const FVector EndTraceBottom = (newLocationForward - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) + GetActorRightVector() * m_capsuleRadious;
  const FVector EndTraceMidle = newLocationForward + GetActorRightVector() * m_capsuleRadious;

  //cambiado a last_position porque va hacia atras y la posicion nueva siempre sera hacia adelante.
  const FVector EndTraceTopBack = (m_lastPosition + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) - GetActorRightVector() * (m_capsuleRadious - 5);
  const FVector EndTraceBottomBack = (m_lastPosition - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) - GetActorRightVector() * (m_capsuleRadious - 5); 
  const FVector EndTraceMidleBack = m_lastPosition - GetActorRightVector() * (m_capsuleRadious - 5);

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("Platform"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bFindInitialOverlaps = true;
  TraceParams.bTraceComplex = true;

  FCollisionResponseParams ResponseParam(ECollisionResponse::ECR_Overlap);

  GetWorld()->LineTraceMulti(OutTraceResultTop, StartTraceTop, EndTraceTop, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionTop = OutTraceResultTop.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceTop, EndTraceTop, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultBottom, StartTraceBottom, EndTraceBottom, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionBottom = OutTraceResultBottom.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceBottom, EndTraceBottom, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  GetWorld()->LineTraceMulti(OutTraceResultMiddle, StartTrace, EndTraceMidle, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionMidle = OutTraceResultMiddle.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceMidle, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  GetWorld()->LineTraceMulti(OutTraceResultTopBack, StartTraceTopBack, EndTraceTopBack, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionTopBack = OutTraceResultTopBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceTopBack, EndTraceTopBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultBottomBack, StartTraceBottomBack, EndTraceBottomBack, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionBottomBack = OutTraceResultBottomBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceBottomBack, EndTraceBottomBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  GetWorld()->LineTraceMulti(OutTraceResultMiddleBack, StartTraceBack, EndTraceMidleBack, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionMidleBack = OutTraceResultMiddleBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceBack, EndTraceMidleBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  if (collisionMidle) {
    int size = OutTraceResultMiddle.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultMiddle[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(GetActorRightVector(), GetActorLocation(), OutTraceResultMiddle[i].Location, m_capsuleRadious));
        ResponseCollision();
        break;
      }
  }
  else if (collisionTop) {
    int size = OutTraceResultTop.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultTop[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(GetActorRightVector(), GetActorLocation(), OutTraceResultTop[i].Location, m_capsuleRadious));
        ResponseCollision();
        break;
      }
  }
  else if (collisionBottom) {
    int size = OutTraceResultBottom.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultBottom[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(GetActorRightVector(), GetActorLocation(), OutTraceResultBottom[i].Location, m_capsuleRadious));
        ResponseCollision();
        break;
      }
  }


  if (collisionMidleBack) {
    int size = OutTraceResultMiddleBack.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultMiddleBack[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(-GetActorRightVector(), GetActorLocation(), OutTraceResultMiddleBack[i].Location, m_capsuleRadious));
        break;
      }
  }
  else if (collisionTopBack) {
    int size = OutTraceResultTopBack.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultTopBack[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(-GetActorRightVector(), GetActorLocation(), OutTraceResultTopBack[i].Location, m_capsuleRadious));
        break;
      }
  }
  else if (collisionBottomBack) {
    int size = OutTraceResultBottomBack.Num();
    for (int i = 0; i < size; i++)
      if (OutTraceResultBottomBack[i].GetActor()->ActorHasTag("Platform")) {
        SetActorLocation(RecalculateLocation(-GetActorRightVector(), GetActorLocation(), OutTraceResultBottomBack[i].Location, m_capsuleRadious));
        break;
      }
  }

  FVector newLocationUp;
  newLocationUp.X = (FMath::Abs(GetActorUpVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationUp.Y = (FMath::Abs(GetActorUpVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationUp.Z = (FMath::Abs(GetActorUpVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  const FVector EndTraceDown = newLocationUp - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownLeftF = (newLocationUp + GetActorRightVector() * m_capsuleRadious) - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownRightF = (newLocationUp - GetActorRightVector() * m_capsuleRadious) - GetActorUpVector() * m_capsuleHeight;

  GetWorld()->LineTraceMulti(OutTraceResultDownLeftF, StartTraceLeftF, EndTraceDownLeftF, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionDownLeftF = OutTraceResultDownLeftF.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceLeftF, EndTraceDownLeftF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultDownRigthF, StartTraceRigthF, EndTraceDownRightF, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionDownRightF = OutTraceResultDownRigthF.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceRigthF, EndTraceDownRightF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultDown, StartTrace, EndTraceDown, COLLISION_ENEMY, TraceParams, ResponseParam);
  bool collisionDown = OutTraceResultDown.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceDown, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  if (collisionDown || collisionDownLeftF || collisionDownRightF) {
    if (collisionDown) {
      int size = OutTraceResultDown.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDown[i].GetActor()->ActorHasTag("Platform")) {
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDown[i].Location, -m_capsuleHeight));
          m_enableGravity = false;
          m_actualJumpSpeed = 0;
          break;
        }
    }
    else if (collisionDownLeftF) {
      int size = OutTraceResultDownLeftF.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDownLeftF[i].GetActor()->ActorHasTag("Platform")) {
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDownLeftF[i].Location, -m_capsuleHeight));
          m_enableGravity = false;
          m_actualJumpSpeed = 0;
          break;
        }
    }
    else if (collisionDownRightF) {
      int size = OutTraceResultDownRigthF.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDownRigthF[i].GetActor()->ActorHasTag("Platform")) {
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDownRigthF[i].Location, -m_capsuleHeight));
          m_enableGravity = false;
          m_actualJumpSpeed = 0;
          break;
        }
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

void AMobileEnemy::ResponseCollision() {
  FVector location = GetActorLocation();
  float val = 0.0f;
  switch (m_state) {
  case TO_RIGHT:
    m_state = RIGHT_DELAY;
    val = (m_rightPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case TO_LEFT:
    m_state = LEFT_DELAY;
    val = (m_leftPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case RIGHT_DELAY:
    val = (m_rightPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case LEFT_DELAY:
    val = (m_leftPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;

  }
}

void AMobileEnemy::ResponseCollisionBackward() {
  FVector location = GetActorLocation();
  float val = 0.0f;
  switch (m_state) {
  case TO_RIGHT:
    val = (m_leftPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case TO_LEFT:
    val = (m_rightPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case RIGHT_DELAY:
    val = (m_leftPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;
  case LEFT_DELAY:
    val = (m_rightPosition - location * m_rightVector).Size();
    m_currentDistance = val;
    break;

  }
}