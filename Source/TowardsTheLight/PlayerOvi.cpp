#include "TowardsTheLight.h"
#include "PlayerOvi.h"
#include "MobilePlatform.h"

/************************************/
/*DEBUG ALTERNATIVO*/

/*if (GEngine)
{
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f, z: %f "), forward.X, forward.Y, forward.Z));
}*/

/*************************************/

APlayerOvi::APlayerOvi() {
  PrimaryActorTick.bCanEverTick = true;
  AutoPossessPlayer = EAutoReceiveInput::Player0;

  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;

  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
  CapsuleComponent->InitCapsuleSize(DEFAULT_CAPSULE_RADIOUS, DEFAULT_CAPSULE_HEIGHT);

  static FName CollisionProfileName(TEXT("Pawn"));
  CapsuleComponent->SetCollisionProfileName(CollisionProfileName);

  CapsuleComponent->CanCharacterStepUpOn = ECB_No;
  CapsuleComponent->bShouldUpdatePhysicsVolume = true;
  CapsuleComponent->bCheckAsyncSceneOnMove = false;
  CapsuleComponent->bCanEverAffectNavigation = false;
  CapsuleComponent->bGenerateOverlapEvents = true;
  //Fijamos por defecto la rotacion de la capsula para que el forward este de cara a la camara
  CapsuleComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 0)));
  CapsuleComponent->SetCapsuleHalfHeight(DEFAULT_CAPSULE_HEIGHT);
  RootComponent = CapsuleComponent;

#if WITH_EDITORONLY_DATA
  ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
  if (ArrowComponent) {
    ArrowComponent->ArrowColor = FColor(150, 200, 255);
    ArrowComponent->bTreatAsASprite = true;
    ArrowComponent->AttachParent = CapsuleComponent;
    ArrowComponent->bIsScreenSizeScaled = true;
  }
#endif // WITH_EDITORONLY_DATA

  Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalComponent"));
  if (Mesh) {
    Mesh->AlwaysLoadOnClient = true;
    Mesh->AlwaysLoadOnServer = true;
    Mesh->bOwnerNoSee = false;
    Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
    Mesh->bCastDynamicShadow = true;
    Mesh->bAffectDynamicIndirectLighting = true;
    Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    Mesh->bChartDistanceFactor = true;
    Mesh->AttachParent = CapsuleComponent;
    static FName CollisionProfileName(TEXT("OverlapAll"));
    Mesh->SetCollisionProfileName(CollisionProfileName);
    Mesh->bGenerateOverlapEvents = true;
    Mesh->bCanEverAffectNavigation = false;
    Mesh->SetRelativeLocation(FVector(0, 0, 0));
    Mesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, 90)));
    Mesh->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
  }

  Stick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stick"));
  Stick->AttachTo(Mesh);

 /* StickLight = CreateDefaultSubobject<ULightComponentBase>(TEXT("StickLight"));
  StickLight->AttachTo(Stick);*/

  m_state = States::RIGHT;
  m_limit = 0;
  m_right = m_left = false;
  m_isJumping = m_doJump = m_hasLanded = m_headCollision = false;
  m_enabledGravity = true;
  
  JumpSpeed = DEFAULT_JUMP_SPEED;
  MovementSpeed = DEFAULT_MOVEMENT_SPEED;
  m_actualJumpSpeed = JumpSpeed;
  AccelerationJump = DEFAULT_JUMP_ACC;
  
  m_capsuleHeight = DEFAULT_CAPSULE_HEIGHT;
  m_capsuleRadious = DEFAULT_CAPSULE_RADIOUS;
  m_capsuleHeightPadding = m_capsuleHeight * PADDING_COLLISION_PERCENT;
  m_capsuleRadiousPadding = m_capsuleRadious * PADDING_COLLISION_PERCENT_RADIOUS;

  m_semiWidthViewPort = 0.f;
  m_centerTouchX = 0.f;
  m_fingerIndexMovement = ETouchIndex::Touch10;
  m_fingerIndexJump = ETouchIndex::Touch10;
  MarginInput = 50.f;
  m_stateInput = States::STOP;
  m_isInJumpTransition = false;
  m_transitionDistance = 0.0f;
  //SpeedIncrementInTransition = DEFAULT_SPEED_TRANSITION;
  
}

void APlayerOvi::BeginPlay(){
  Super::BeginPlay();

  float dotForward = FVector::DotProduct(FVector(1, 1, 1), GetActorForwardVector());
  if (dotForward < 0)
    m_state = States::LEFT;
  else
    m_state = States::RIGHT;

  this->Tags.Add("Player");
  m_limit = FVector::DotProduct(GetActorLocation(), GetActorRightVector());
  m_limit = abs(m_limit);

  //GetCaspuleValues
  m_capsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
  m_capsuleRadious = CapsuleComponent->GetScaledCapsuleRadius();
  m_capsuleHeightPadding = m_capsuleHeight * PADDING_COLLISION_PERCENT;
  m_capsuleRadiousPadding = m_capsuleRadious * PADDING_COLLISION_PERCENT_RADIOUS;
}

void APlayerOvi::Tick(float DeltaTime){
  Super::Tick(DeltaTime);

  if (m_semiWidthViewPort == 0)
    m_semiWidthViewPort = GEngine->GameViewport->Viewport->GetSizeXY().X / 2;

  float value = UpdateState();
  DoMovement(DeltaTime, value);
  DoJump(DeltaTime);
  CalculateGravity(DeltaTime);
  CalculateOrientation();
  CheckCollision();
  CheckMobilePlatform();
}

float APlayerOvi::UpdateState() {
  m_lastPosition = GetActorLocation();
  float value = 0;
  FVector up = GetActorUpVector();
  FVector forward = GetActorForwardVector();

  if (m_right){
    value = 1;
    if (m_state != States::RIGHT){
      Rotate(FVector(0, 0, -180));
      m_state = States::RIGHT;
    }
    
  }
  else if (m_left){
    value = 1;
    if (m_state != States::LEFT){
      Rotate(FVector(0, 0, 180));
      m_state = States::LEFT;
    }
  }
  else{
    value = 0;
  }

  return value;
}

void APlayerOvi::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
  Super::SetupPlayerInputComponent(InputComponent);
  InputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerOvi::OnStartRight);
  InputComponent->BindAction("MoveRight", IE_Released, this, &APlayerOvi::OnStopRight);
  InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerOvi::OnStartLeft);
  InputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerOvi::OnStopLeft);
  InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerOvi::OnStartJump);
  InputComponent->BindAction("Jump", IE_Released, this, &APlayerOvi::OnStopJump);

  InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerOvi::TouchStarted);
  InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APlayerOvi::TouchStarted);
  InputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerOvi::TouchEnd);
}

void APlayerOvi::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location) {
  if (Location.X > m_semiWidthViewPort && m_fingerIndexMovement != FingerIndex) { //JUMP
    OnStartJump();
    m_fingerIndexJump = FingerIndex;
    //if (!nearGear)
    //  OnStartJump();
    //else{
    //  puntero a nearGear->doAction();
    //}
  }
  else { //MOVEMENT SWIPE
    if (m_centerTouchX == 0){ //initial touch
      m_centerTouchX = Location.X;
      m_fingerIndexMovement = FingerIndex;
    }
    if (m_fingerIndexMovement == FingerIndex){
      if (Location.X > m_centerTouchX + MarginInput){ //right swipe
        OnStartRight();
        OnStopLeft();
        m_centerTouchX = Location.X - MarginInput;
        m_stateInput = States::RIGHT;
      }
      else if (Location.X < m_centerTouchX - MarginInput){ //left swipe
        OnStartLeft();
        OnStopRight();
        m_centerTouchX = Location.X + MarginInput;
        m_stateInput = States::LEFT;
      }
      else if (Location.X > m_centerTouchX && m_stateInput == States::RIGHT){ //right swipe
        OnStartRight();
        OnStopLeft();
      }
      else if (Location.X < m_centerTouchX && m_stateInput == States::LEFT){ //left swipe
        OnStartLeft();
        OnStopRight();
      }
      else{ //no movement
        OnStopRight();
        OnStopLeft();
        m_stateInput = States::STOP;
      }
    }
  }
}

void APlayerOvi::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location) {
  if (FingerIndex == m_fingerIndexJump){
    OnStopJump();
    m_fingerIndexJump = ETouchIndex::Touch10;
  }

  if (FingerIndex == m_fingerIndexMovement){
    m_centerTouchX = 0.f;
    m_fingerIndexMovement = ETouchIndex::Touch10;
    OnStopRight();
    OnStopLeft();
  }
}

void APlayerOvi::OnStartRight() {
  m_right = true;
}

void APlayerOvi::OnStopRight() {
  m_right = false;
}

void APlayerOvi::OnStartLeft() {
  m_left = true;
}

void APlayerOvi::OnStopLeft() {
  m_left = false;
}

void APlayerOvi::OnStartJump() {
  m_doJump = true;
}

void APlayerOvi::OnStopJump() {
  m_doJump = false;
}

void  APlayerOvi::DoMovement(float DeltaTime, float value){
  FVector forward = GetActorForwardVector();
  FVector location = GetActorLocation();
  location += MovementSpeed * DeltaTime * value * forward;
  SetActorLocation(location);
}

void APlayerOvi::DoJump(float DeltaTime){
  if (m_hasLanded && !m_doJump) {
    m_isJumping = false;
    m_headCollision = false;
  }

  if (m_doJump && !m_isJumping && m_hasLanded) {
    m_hasLanded = false;
    m_isJumping = true;
    m_headCollision = false;
    m_doJump = false;
  }

  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  // movimiento uniformemente acelerado con aceleración AccelerationJump caidas libres
  if (m_isJumping && !m_headCollision) {
    if (m_actualJumpSpeed > 0) {
      if (!m_isInJumpTransition){
        m_actualJumpSpeed -= AccelerationJump * DeltaTime;
      }
      else {
        m_transitionDistance += m_actualJumpSpeed * DeltaTime;
        if (m_transitionDistance >= 200.0f)
          m_isInJumpTransition = false;
      }
      m_enabledGravity = false;
      location += m_actualJumpSpeed * DeltaTime * up;
    }
    else {
      m_isJumping = false;
    }
  } else {
    m_enabledGravity = true;
  }

  SetActorLocation(location);
}

void APlayerOvi::CalculateGravity(float DeltaTime){
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  if (m_enabledGravity) {
    if (m_actualJumpSpeed < JumpSpeed)
      m_actualJumpSpeed += AccelerationJump * DeltaTime;
    else{
      m_actualJumpSpeed = JumpSpeed;
    }
    location -= m_actualJumpSpeed * DeltaTime * up;
    SetActorLocation(location);
  }
}

void APlayerOvi::CalculateOrientation(){
  FVector forward = GetActorForwardVector();
  FVector up = GetActorUpVector();

  float dotForward = FVector::DotProduct(GetActorLocation(), forward);

  if (dotForward > m_limit && m_state == States::RIGHT)
    Rotate(FVector(0, 0, -90));

  if (dotForward > m_limit && m_state == States::LEFT)
    Rotate(FVector(0, 0, 90));

  float dotUp = FVector::DotProduct(GetActorLocation(), up);
  float val = 0;

  if (dotUp > m_limit || dotUp < -m_limit) {
    bool toUp = dotUp > m_limit;

    val = (toUp) ? 90 : -90;

    if (toUp && m_isJumping) {
      m_isInJumpTransition = true;
      m_transitionDistance = 0.0f;
    }

    if (m_state == States::RIGHT)
      Rotate(FVector(-val, 0, 0));
    else if (m_state == States::LEFT)
      Rotate(FVector(val, 0, 0));
  }

  AjustPosition();
}

void APlayerOvi::AjustPosition() {
  FVector location = GetActorLocation();

  if (location.X > m_limit)
    location.X = m_limit;
  else if (location.X < -m_limit)
    location.X = -m_limit;

  if (location.Y > m_limit)
    location.Y = m_limit;
  else if (location.Y < -m_limit)
    location.Y = -m_limit;

  if (location.Z > m_limit)
    location.Z = m_limit;
  else if (location.Z < -m_limit)
    location.Z = -m_limit;

  SetActorLocation(location);
}

void APlayerOvi::CheckCollision() {
  TArray<FHitResult> OutTraceResultDown;
  TArray<FHitResult> OutTraceResultDownLeftF;
  TArray<FHitResult> OutTraceResultDownRigthF;

  TArray<FHitResult> OutTraceResultUp;
  TArray<FHitResult> OutTraceResultUpLeftF;
  TArray<FHitResult> OutTraceResultUpRigthF;

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

  FVector StartTraceLeftF = StartTrace + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);
  FVector StartTraceRigthF = StartTrace - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);

  // Calculate the end location for trace  
  //horizontal
  FVector newLocationForward;
  newLocationForward.X = (FMath::Abs(GetActorForwardVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationForward.Y = (FMath::Abs(GetActorForwardVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationForward.Z = (FMath::Abs(GetActorForwardVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;
  
  const FVector EndTraceTop = (newLocationForward + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) + GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceBottom = (newLocationForward - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) + GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceMidle = newLocationForward + GetActorForwardVector() * m_capsuleRadious;

  const FVector EndTraceTopBack = (newLocationForward + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) - GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceBottomBack = (newLocationForward - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding)) - GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceMidleBack = newLocationForward - GetActorForwardVector() * m_capsuleRadious;

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("Platform"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bFindInitialOverlaps = false;
  TraceParams.bTraceComplex = true;
  
  FCollisionResponseParams ResponseParam(ECollisionResponse::ECR_Overlap);

  GetWorld()->LineTraceMulti(OutTraceResultTop, StartTraceTop, EndTraceTop, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionTop = OutTraceResultTop.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceTop, EndTraceTop, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultBottom, StartTraceBottom, EndTraceBottom, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionBottom = OutTraceResultBottom.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceBottom, EndTraceBottom, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultMiddle, StartTrace, EndTraceMidle, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionMidle = OutTraceResultMiddle.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceMidle, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);


  GetWorld()->LineTraceMulti(OutTraceResultTopBack, StartTraceTop, EndTraceTopBack, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionTopBack = OutTraceResultTopBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceTop, EndTraceTopBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultBottomBack, StartTraceBottom, EndTraceBottomBack, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionBottomBack = OutTraceResultBottomBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceBottom, EndTraceBottomBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultMiddleBack, StartTrace, EndTraceMidleBack, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionMidleBack = OutTraceResultMiddleBack.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceMidleBack, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);

  if (collisionTop || collisionBottom || collisionMidle) {

    if (collisionTop) {
      int size = OutTraceResultTop.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultTop[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(GetActorForwardVector(), GetActorLocation(), OutTraceResultTop[i].Location, m_capsuleRadious));
    }
    else if (collisionBottom) {
      int size = OutTraceResultBottom.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultBottom[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(GetActorForwardVector(), GetActorLocation(), OutTraceResultBottom[i].Location, m_capsuleRadious));
    }
    else if (collisionMidle) {
      int size = OutTraceResultMiddle.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultMiddle[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(GetActorForwardVector(), GetActorLocation(), OutTraceResultMiddle[i].Location, m_capsuleRadious));
    }
  }

  if (collisionTopBack || collisionBottomBack || collisionMidleBack) {
    if (collisionTopBack) {
      int size = OutTraceResultTopBack.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultTopBack[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(-GetActorForwardVector(), GetActorLocation(), OutTraceResultTopBack[i].Location, m_capsuleRadious));
    }
    else if (collisionBottomBack) {
      int size = OutTraceResultBottomBack.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultBottomBack[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(-GetActorForwardVector(), GetActorLocation(), OutTraceResultBottomBack[i].Location, m_capsuleRadious));
    }
    else if (collisionMidleBack) {
      int size = OutTraceResultMiddleBack.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultMiddleBack[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(-GetActorForwardVector(), GetActorLocation(), OutTraceResultMiddleBack[i].Location, m_capsuleRadious));
    }
  }

  // Calculate the end location for trace  
  // Vertical 
  const FVector EndTraceDown = GetActorLocation() - GetActorUpVector() * m_capsuleHeight;
  //esta posicion es coger la posicion actual pero solo en la cordenada deseada, ya que si no el rayo seria oblicuo.
  FVector newLocationUp;
  newLocationUp.X = (FMath::Abs(GetActorUpVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationUp.Y = (FMath::Abs(GetActorUpVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationUp.Z = (FMath::Abs(GetActorUpVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  const FVector EndTraceDownLeftF = (newLocationUp + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownRightF = (newLocationUp - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) - GetActorUpVector() * m_capsuleHeight;

  const FVector EndTraceUp = newLocationUp + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpLeftF = (newLocationUp + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpRightF = (newLocationUp - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) + GetActorUpVector() * m_capsuleHeight;

  GetWorld()->LineTraceMulti(OutTraceResultDown, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDown = OutTraceResultDown.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceDown, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultDownLeftF, StartTraceLeftF, EndTraceDownLeftF, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDownLeftF = OutTraceResultDownLeftF.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceLeftF, EndTraceDownLeftF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  GetWorld()->LineTraceMulti(OutTraceResultDownRigthF, StartTraceRigthF, EndTraceDownRightF, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDownRightF = OutTraceResultDownRigthF.Num() > 0;
  //DrawDebugLine(GetWorld(), StartTraceRigthF, EndTraceDownRightF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);


  GetWorld()->LineTraceMulti(OutTraceResultUp, StartTrace, EndTraceUp, COLLISION_PLAYER, TraceParams, ResponseParam);
  //DrawDebugLine(GetWorld(), StartTrace, EndTraceUp, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  bool collisionUp = OutTraceResultUp.Num() > 0;
  GetWorld()->LineTraceMulti(OutTraceResultUpLeftF, StartTraceLeftF, EndTraceUpLeftF, COLLISION_PLAYER, TraceParams, ResponseParam);
  //DrawDebugLine(GetWorld(), StartTraceLeftF, EndTraceUpLeftF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  bool collisionUpLeftF = OutTraceResultUp.Num() > 0;
  GetWorld()->LineTraceMulti(OutTraceResultUpRigthF, StartTraceRigthF, EndTraceUpRightF, COLLISION_PLAYER, TraceParams, ResponseParam);
  //DrawDebugLine(GetWorld(), StartTraceRigthF, EndTraceUpRightF, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  bool collisionUpRightF = OutTraceResultUp.Num() > 0;

  if (collisionDown || collisionDownLeftF || collisionDownRightF) {

    if (collisionDown) {
      int size = OutTraceResultDown.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDown[i].GetActor()->ActorHasTag("Platform")) 
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDown[i].Location, -m_capsuleHeight));
    }
    else if (collisionDownLeftF) {
      int size = OutTraceResultDownLeftF.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDownLeftF[i].GetActor()->ActorHasTag("Platform"))
          SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDownLeftF[i].Location, -m_capsuleHeight));
    }
    else if (collisionDownRightF) {
      int size = OutTraceResultDownRigthF.Num();
      for (int i = 0; i < size; i++)
          if (OutTraceResultDownRigthF[i].GetActor()->ActorHasTag("Platform")) 
            SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultDownRigthF[i].Location, -m_capsuleHeight));
    }
    m_hasLanded = true;
    m_actualJumpSpeed = JumpSpeed;
  }
  else {
    m_hasLanded = false;
    if (collisionUp || collisionUpLeftF || collisionUpRightF) {
      if (collisionUp) {
        int size = OutTraceResultUp.Num();
        for (int i = 0; i < size; i++)
          if (OutTraceResultUp[i].GetActor()->ActorHasTag("Platform"))
            SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultUp[i].Location, m_capsuleHeight));
      }
      else if (collisionUpLeftF) {
        int size = OutTraceResultUpLeftF.Num();
        for (int i = 0; i < size; i++) 
          if (OutTraceResultUpLeftF[i].GetActor()->ActorHasTag("Platform")) 
            SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultUpLeftF[i].Location, m_capsuleHeight));
      }
      else if (collisionUpRightF) {
        int size = OutTraceResultUpRigthF.Num();
        for (int i = 0; i < size; i++)
          if (OutTraceResultUpRigthF[i].GetActor()->ActorHasTag("Platform"))
            SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation(), OutTraceResultUpRigthF[i].Location, m_capsuleHeight));
      }
      m_headCollision = true;
      m_actualJumpSpeed = 0.0f;
    }
  }
}

void APlayerOvi::CheckMobilePlatform() {
  TArray<FHitResult> OutTraceResultDown;
  TArray<FHitResult> OutTraceResultDownLeftF;
  TArray<FHitResult> OutTraceResultDownRigthF;

  // Calculate the start location for trace  
  FVector StartTrace = m_lastPosition;
  FVector StartTraceLeftF = StartTrace + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);
  FVector StartTraceRigthF = StartTrace - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);

  // Calculate endpoint of trace  
  const FVector EndTraceDown = GetActorLocation() - GetActorUpVector() * m_capsuleHeight;

  FVector newLocationUp;
  newLocationUp.X = (FMath::Abs(GetActorUpVector().X) <= 0.01) ? m_lastPosition.X : GetActorLocation().X;
  newLocationUp.Y = (FMath::Abs(GetActorUpVector().Y) <= 0.01) ? m_lastPosition.Y : GetActorLocation().Y;
  newLocationUp.Z = (FMath::Abs(GetActorUpVector().Z) <= 0.01) ? m_lastPosition.Z : GetActorLocation().Z;

  const FVector EndTraceDownLeftF = (newLocationUp + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownRightF = (newLocationUp - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding)) - GetActorUpVector() * m_capsuleHeight;

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("Platform"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bFindInitialOverlaps = false;
  TraceParams.bTraceComplex = true;

  FCollisionResponseParams ResponseParam(ECollisionResponse::ECR_Overlap);

  GetWorld()->LineTraceMulti(OutTraceResultDown, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDown = OutTraceResultDown.Num() > 0;
  GetWorld()->LineTraceMulti(OutTraceResultDownLeftF, StartTraceLeftF, EndTraceDownLeftF, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDownLeftF = OutTraceResultDownLeftF.Num() > 0;
  GetWorld()->LineTraceMulti(OutTraceResultDownRigthF, StartTraceRigthF, EndTraceDownRightF, COLLISION_PLAYER, TraceParams, ResponseParam);
  bool collisionDownRightF = OutTraceResultDownRigthF.Num() > 0;

  if (collisionDown || collisionDownLeftF || collisionDownRightF) {

    if (collisionDown) {
      int size = OutTraceResultDown.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDown[i].GetActor()->ActorHasTag("MobilePlatform")) {
          AMobilePlatform *mobile = dynamic_cast<AMobilePlatform *>(OutTraceResultDown[i].GetActor());
          if (mobile) {
            FVector mov = mobile->GetPlatformMovement();
            FVector loc = GetActorLocation();
            SetActorLocation(loc + mov);
          }
        }
    }
    else if (collisionDownLeftF) {
      int size = OutTraceResultDownLeftF.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDownLeftF[i].GetActor()->ActorHasTag("MobilePlatform")) {
          AMobilePlatform *mobile = dynamic_cast<AMobilePlatform *>(OutTraceResultDownLeftF[i].GetActor());
          if (mobile) {
            FVector mov = mobile->GetPlatformMovement();
            FVector loc = GetActorLocation();
            SetActorLocation(loc + mov);
          }
        }
    }
    else if (collisionDownRightF) {
      int size = OutTraceResultDownRigthF.Num();
      for (int i = 0; i < size; i++)
        if (OutTraceResultDownRigthF[i].GetActor()->ActorHasTag("MobilePlatform")) {
          AMobilePlatform *mobile = dynamic_cast<AMobilePlatform *>(OutTraceResultDownRigthF[i].GetActor());
          if (mobile) {
            FVector mov = mobile->GetPlatformMovement();
            FVector loc = GetActorLocation();
            SetActorLocation(loc + mov);
          }
        }
    }
  }
}

void APlayerOvi::Rotate(const FVector& rotation) {
  FTransform transform = GetTransform();
  FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(rotation);
  transform.SetRotation(q);
  SetActorTransform(transform);
}

FVector APlayerOvi::AbsVector(const FVector& vector) {
  FVector absVector = FVector::ZeroVector;

  absVector.X = (vector.X < 0) ? -vector.X : vector.X;
  absVector.Y = (vector.Y < 0) ? -vector.Y : vector.Y;
  absVector.Z = (vector.Z < 0) ? -vector.Z : vector.Z;

  return absVector;
}

FVector APlayerOvi::RecalculateLocation(FVector Direction, FVector Location, FVector HitLocation, float size)
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


