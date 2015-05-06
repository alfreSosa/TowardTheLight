#include "TowardsTheLight.h"
#include "PlayerOvi.h"

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
  CapsuleComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, -180)));
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
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(CollisionProfileName);
		Mesh->bGenerateOverlapEvents = true;
		Mesh->bCanEverAffectNavigation = false;
    Mesh->SetRelativeLocation(FVector(0, 0, -95));
    Mesh->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0, 0, -90)));

	}

	m_state = States::STOP;
	m_limit = 0;
  m_jumpDistance = 0;
  m_right = m_left = 0;
	m_isJumping = m_doJump = m_hasLanded = m_headCollision = false;
  m_enabledGravity = true;
  JumpSpeed = MovementSpeed = DEFAULT_MOVEMENT_SPEED;
  MaxJumpHeight = DEFAULT_JUMP_HEIGHT;
  m_capsuleHeight = DEFAULT_CAPSULE_HEIGHT;
  m_capsuleRadious = DEFAULT_CAPSULE_RADIOUS;
  m_capsuleHeightPadding = m_capsuleHeight * PADDING_COLLISION_PERCENT;
  m_capsuleRadiousPadding = m_capsuleRadious * PADDING_COLLISION_PERCENT;

}

void APlayerOvi::BeginPlay(){
	Super::BeginPlay();
 
	this->Tags.Add("Player");
	m_limit = FVector::DotProduct(GetActorLocation(), GetActorForwardVector());
	m_limit = abs(m_limit);	
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
  m_fingerIndexRight = m_fingerIndexLeft = ETouchIndex::Touch10;
  //GetCaspuleValues
  m_capsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
  m_capsuleRadious = CapsuleComponent->GetScaledCapsuleRadius();
  m_capsuleHeightPadding = m_capsuleHeight * PADDING_COLLISION_PERCENT;
  m_capsuleRadiousPadding = m_capsuleRadious * PADDING_COLLISION_PERCENT;
}

void APlayerOvi::Tick(float DeltaTime){
  Super::Tick(DeltaTime);

  InputManager();
  float value = UpdateState();
  DoMovement(DeltaTime, value);
  DoJump(DeltaTime);
  CalculateGravity(DeltaTime);
  CalculateOrientation();
  CheckCollision();
}

float APlayerOvi::UpdateState() {
  m_lastPosition = GetActorLocation();
  float value = 0;
  FVector up = GetActorUpVector();
  FVector forward = GetActorForwardVector();

  if (m_right > m_frameToMove && m_left <= m_frameToMove) {
    value = 1;
    if (m_state != States::RIGHT)
      Rotate(FVector(0, 0, -90));

    m_state = States::RIGHT;
  }
  else if (m_left > m_frameToMove && m_right <= m_frameToMove) {
    value = 1;
    if (m_state != States::LEFT)
      Rotate(FVector(0, 0, 90));

    m_state = States::LEFT;
  }
  else if (m_left <= m_frameToMove && m_right <= m_frameToMove) {
    value = 0;
    if (m_state == States::RIGHT)
      Rotate(FVector(0, 0, 90));

    if (m_state == States::LEFT)
      Rotate(FVector(0, 0, -90));

    m_state = States::STOP;
  }
  else {
    if (m_state == States::RIGHT || m_state == States::LEFT)
      value = 1;
  }
  return value;
}

void APlayerOvi::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
  Super::SetupPlayerInputComponent(InputComponent);
  InputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerOvi::OnStartRight);
  InputComponent->BindAction("MoveRight", IE_Released, this, &APlayerOvi::OnStopRight);
  InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerOvi::OnStartLeft);
  InputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerOvi::OnStopLeft);
  InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerOvi::TouchStarted);
  InputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerOvi::TouchEnd);
}

void APlayerOvi::InputManager(){
  m_right = (m_startRight) ? m_right + 1 : 0;
  m_left = (m_startLeft) ? m_left + 1 : 0;

  m_doJump = false;
  if (m_right && m_left)
    m_doJump = true;
}

void APlayerOvi::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location) {
  FVector loc = Location;
  loc.Normalize();

  if (loc.X > 0.5) {
    m_startRight = true;
    m_fingerIndexRight = FingerIndex;
  }
  else if (loc.X < 0.5) {
    m_startLeft = true;
    m_fingerIndexLeft = FingerIndex;
  }

  if (GEngine) {
    if (FingerIndex != ETouchIndex::Touch1 && FingerIndex != ETouchIndex::Touch2)
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Efinferindex > 2")));
  }
}

void APlayerOvi::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location) {
  if (FingerIndex == m_fingerIndexRight) {
    m_startRight = false;
    m_fingerIndexRight = ETouchIndex::Touch10;
  }
  else if (FingerIndex == m_fingerIndexLeft) {
    m_startLeft = false;
    m_fingerIndexLeft = ETouchIndex::Touch10;

  }
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH END")));
  }
}


void APlayerOvi::OnStartRight() {
  m_startRight = true;
}

void APlayerOvi::OnStopRight() {
  m_startRight = false;
}

void APlayerOvi::OnStartLeft() {
  m_startLeft = true;
}

void APlayerOvi::OnStopLeft() {
  m_startLeft = false;
}

void  APlayerOvi::DoMovement(float DeltaTime, float value){
  FVector forward = GetActorForwardVector();
  FVector location = GetActorLocation();
  location += MovementSpeed * DeltaTime * value * forward;
  SetActorLocation(location);
}

void APlayerOvi::DoJump(float DeltaTime){
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  if (m_hasLanded && !m_doJump) {
    m_isJumping = false;
    m_jumpDistance = 0.0;
    m_headCollision = false;
  }

  if (m_doJump && !m_isJumping && m_hasLanded) {
    m_hasLanded = false;
    m_isJumping = true;
    m_headCollision = false;
  }

  if (m_isJumping && !m_headCollision) {
    m_jumpDistance += JumpSpeed * DeltaTime;
    if (m_jumpDistance < MaxJumpHeight)
      location += JumpSpeed * 2 * DeltaTime * up;
  }

  SetActorLocation(location);
}

void APlayerOvi::CalculateGravity(float DeltaTime){
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  location -= JumpSpeed * DeltaTime * up;
  SetActorLocation(location);
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
    
    if (toUp && m_isJumping)
      m_jumpDistance -= DEFAULT_JUMP_TRANSITION;

    if (m_state == States::STOP)
      Rotate(FVector(0, val, 0));
    else if (m_state == States::RIGHT)
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

void APlayerOvi::CheckCollision(){
  FHitResult OutTraceResult;

  // Calculate the start location for trace  
  FVector StartTrace = m_lastPosition;
  FVector StartTraceTop = StartTrace + GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding); // REVISAR ESTAS COSTANTES
  FVector StartTraceBottom = StartTrace - GetActorUpVector() * (m_capsuleHeight - m_capsuleHeightPadding);

  FVector StartTraceLeft = StartTrace - GetActorRightVector() * (m_capsuleRadious - m_capsuleRadiousPadding);
  FVector StartTraceRigth = StartTrace + GetActorRightVector() * (m_capsuleRadious - m_capsuleRadiousPadding);
  FVector StartTraceLeftF = StartTrace - GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);
  FVector StartTraceRigthF = StartTrace + GetActorForwardVector() * (m_capsuleRadious - m_capsuleRadiousPadding);

  // Calculate endpoint of trace  
  FVector difPositionUp = (GetActorLocation() - m_lastPosition) * AbsVector(GetActorUpVector());
  FVector difPositionForward = (GetActorLocation() - GetActorLocation()) * AbsVector(GetActorForwardVector());

  const FVector EndTraceDown = StartTrace + difPositionUp - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownLeft = StartTraceLeft + difPositionUp - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownRight = StartTraceRigth + difPositionUp - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownLeftF = StartTraceLeftF + difPositionUp - GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceDownRightF = StartTraceRigthF + difPositionUp - GetActorUpVector() * m_capsuleHeight;

  const FVector EndTraceUp = StartTrace + difPositionUp + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpLeft = StartTraceLeft + difPositionUp + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpRight = StartTraceRigth + difPositionUp + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpLeftF = StartTraceLeftF + difPositionUp + GetActorUpVector() * m_capsuleHeight;
  const FVector EndTraceUpRightF = StartTraceRigthF + difPositionUp + GetActorUpVector() * m_capsuleHeight;
  //horizontal
  const FVector EndTraceTop = StartTraceTop + difPositionForward + GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceBottom = StartTraceBottom + difPositionForward + GetActorForwardVector() * m_capsuleRadious;
  const FVector EndTraceMidle = StartTrace + difPositionForward + GetActorForwardVector() * m_capsuleRadious;

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("ColliderTrace"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;

  bool collisionDown = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams);
 // DrawDebugLine(GetWorld(), StartTrace, EndTraceDown, FColor(1.0f, 0.f, 0.f, 1.f), false, 10.f);
  bool collisionDownLeft = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceLeft, EndTraceDownLeft, COLLISION_PLAYER, TraceParams);
  bool collisionDownRight = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceRigth, EndTraceDownRight, COLLISION_PLAYER, TraceParams);
  bool collisionDownLeftF = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceLeftF, EndTraceDownLeftF, COLLISION_PLAYER, TraceParams);
  bool collisionDownRightF = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceRigthF, EndTraceDownRightF, COLLISION_PLAYER, TraceParams);

  bool collisionUp = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceUp, COLLISION_PLAYER, TraceParams);
  bool collisionUpLeft = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceLeft, EndTraceUpLeft, COLLISION_PLAYER, TraceParams);
  bool collisionUpRight = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceRigth, EndTraceUpRight, COLLISION_PLAYER, TraceParams);
  bool collisionUpLeftF = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceLeftF, EndTraceUpLeftF, COLLISION_PLAYER, TraceParams);
  bool collisionUpRightF = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceRigthF, EndTraceUpRightF, COLLISION_PLAYER, TraceParams);
  
  bool collisionTop = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceTop, EndTraceTop, COLLISION_PLAYER, TraceParams);
  bool collisionBottom = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceBottom, EndTraceBottom, COLLISION_PLAYER, TraceParams);
  bool collisionMidle = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceMidle, COLLISION_PLAYER, TraceParams);


  if (collisionDown || collisionDownLeft || collisionDownRight || collisionDownLeftF || collisionDownRightF) {
    SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation()));
    m_hasLanded = true;
  }
  else {
    m_hasLanded = false;
  }

  if (collisionUp || collisionUpLeft || collisionUpRight || collisionUpLeftF || collisionUpRightF) {
    SetActorLocation(RecalculateLocation(GetActorUpVector(), GetActorLocation()));
    m_headCollision = true;
  }

  if (collisionTop || collisionBottom || collisionMidle)
    SetActorLocation(RecalculateLocation(GetActorForwardVector(), GetActorLocation()));
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

FVector APlayerOvi::RecalculateLocation(FVector direction, FVector Location)
{
  FVector loc = Location;
  FVector absDirection = FVector::ZeroVector;
  FVector dir = direction;

  absDirection.X = (FMath::Abs(dir.X) <= 0.01) ? 0 : 1;
  absDirection.Y = (FMath::Abs(dir.Y) <= 0.01) ? 0 : 1;
  absDirection.Z = (FMath::Abs(dir.Z) <= 0.01) ? 0 : 1;

  FVector nPos = absDirection * m_lastPosition;

  loc.X = (FMath::Abs(nPos.X) <= 0.01) ? loc.X : nPos.X;
  loc.Y = (FMath::Abs(nPos.Y) <= 0.01) ? loc.Y : nPos.Y;
  loc.Z = (FMath::Abs(nPos.Z) <= 0.01) ? loc.Z : nPos.Z;

  return loc;
}


