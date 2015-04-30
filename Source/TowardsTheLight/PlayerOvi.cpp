// Fill out your copyright notice in the Description page of Project Settings.

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
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);

	static FName CollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetCollisionProfileName(CollisionProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->bShouldUpdatePhysicsVolume = true;
	CapsuleComponent->bCheckAsyncSceneOnMove = false; 
	CapsuleComponent->bCanEverAffectNavigation = false;
  CapsuleComponent->bGenerateOverlapEvents = true;
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
	}

	m_state = States::STOP;
	m_limit = 0;
  m_jumpDistance = 0;
	m_isJumping = m_doJump = m_hasLanded = m_headCollision = false;
  m_enabledGravity = true;
  JumpSpeed = MovementSpeed = 300.0f;
  MaxJumpHeight = 150.0f;
  m_rotation = FVector::ZeroVector;

	FName *name = new FName("player", EFindName::FNAME_Add);
	FName f = GetFName();
	
}

void APlayerOvi::BeginPlay()
{
	Super::BeginPlay();
 
	this->Tags.Add("Player");
	m_limit = FVector::DotProduct(GetActorLocation(), GetActorForwardVector());
	m_limit = abs(m_limit);	
  m_rotation = GetActorRotation().Euler();
  m_lastRotation = FVector::ZeroVector;
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	m_viewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
	
}

void APlayerOvi::CheckCollision()
{

  FHitResult OutTraceResult;

  // Calculate the start location for trace  
  FVector StartTrace = GetActorLocation();

  float capsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
  float capsuleRadious = CapsuleComponent->GetScaledCapsuleRadius();
  FVector StartTraceTop = GetActorLocation() + GetActorUpVector() * (capsuleHeight - 2.0f);
  FVector StartTraceBottom = GetActorLocation() - GetActorUpVector() * (capsuleHeight - 10.0f);

  // Calculate endpoint of trace  
  const FVector EndTraceDown = StartTrace - GetActorUpVector() * capsuleHeight;
  const FVector EndTraceUp = StartTrace + GetActorUpVector() * capsuleHeight;
  //horizontal
  const FVector EndTraceTop = StartTraceTop + GetActorForwardVector() * capsuleRadious;
  const FVector EndTraceBottom = StartTraceBottom + GetActorForwardVector() * capsuleRadious;
  const FVector EndTraceMidle = StartTrace + GetActorForwardVector() * capsuleRadious;

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("ColliderTrace"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;

  bool collisionDown = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams);
  bool collisionUp = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceUp, COLLISION_PLAYER, TraceParams);

  bool collisionTop = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceTop, EndTraceTop, COLLISION_PLAYER, TraceParams);
  bool collisionBottom = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceBottom, EndTraceBottom, COLLISION_PLAYER, TraceParams);
  bool collisionMidle = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceMidle, COLLISION_PLAYER, TraceParams);

  if (collisionDown) {
    FVector loc = GetActorLocation();
    FVector absUp = FVector::ZeroVector;
    FVector up = GetActorUpVector();

    absUp.X = (FMath::Abs(up.X) <= 0.01) ? 0 : 1;
    absUp.Y = (FMath::Abs(up.Y) <= 0.01) ? 0 : 1;
    absUp.Z = (FMath::Abs(up.Z) <= 0.01) ? 0 : 1;

    FVector upPosition = absUp * m_lastPosition;

    loc.X = (FMath::Abs(upPosition.X) <= 0.01) ? loc.X : upPosition.X;
    loc.Y = (FMath::Abs(upPosition.Y) <= 0.01) ? loc.Y : upPosition.Y;
    loc.Z = (FMath::Abs(upPosition.Z) <= 0.01) ? loc.Z : upPosition.Z;
    SetActorLocation(loc);

    m_hasLanded = true;
  }

  if (collisionUp) {
    FVector loc = GetActorLocation();

    FVector absUp = FVector::ZeroVector;
    FVector up = GetActorUpVector();

    absUp.X = (FMath::Abs(up.X) <= 0.01) ? 0 : 1;
    absUp.Y = (FMath::Abs(up.Y) <= 0.01) ? 0 : 1;
    absUp.Z = (FMath::Abs(up.Z) <= 0.01) ? 0 : 1;

    FVector upPosition = absUp * m_lastPosition;


    loc.X = (FMath::Abs(upPosition.X) <= 0.01) ? loc.X : upPosition.X;
    loc.Y = (FMath::Abs(upPosition.Y) <= 0.01) ? loc.Y : upPosition.Y;
    loc.Z = (FMath::Abs(upPosition.Z) <= 0.01) ? loc.Z : upPosition.Z;
    SetActorLocation(loc);
    m_headCollision = true;
  }

  if (collisionTop || collisionBottom || collisionMidle) {
    FVector loc = GetActorLocation();
    FVector absForward = FVector::ZeroVector;
    FVector forward = GetActorForwardVector();

    absForward.X = (FMath::Abs(forward.X) <= 0.01) ? 0 : 1;
    absForward.Y = (FMath::Abs(forward.Y) <= 0.01) ? 0 : 1;
    absForward.Z = (FMath::Abs(forward.Z) <= 0.01) ? 0 : 1;

    FVector forPosition = absForward * m_lastPosition;

    loc.X = (FMath::Abs(forPosition.X) <= 0.01) ? loc.X : forPosition.X;
    loc.Y = (FMath::Abs(forPosition.Y) <= 0.01) ? loc.Y : forPosition.Y;
    loc.Z = (FMath::Abs(forPosition.Z) <= 0.01) ? loc.Z : forPosition.Z;
    SetActorLocation(loc);
  }
}

void APlayerOvi::CalculateOrientation()
{
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

    if (m_state == States::STOP)
      Rotate(FVector(0, val, 0));
    else if (m_state == States::RIGHT)
      Rotate(FVector(-val, 0, 0));
    else if (m_state == States::LEFT)
      Rotate(FVector(val, 0, 0));
  }

  AjustPosition();

}

void APlayerOvi::DoJump(float DeltaTime)
{
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  if (m_hasLanded && !m_doJump) {
    m_isJumping = false;
    m_jumpDistance = 0.0;
    m_headCollision = false;
  }

  if (m_doJump && !m_isJumping) {
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

void APlayerOvi::CalculateGravity(float DeltaTime)
{
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  location -= JumpSpeed * DeltaTime * up;
  SetActorLocation(location);
}

void  APlayerOvi::DoMovement(float DeltaTime, float value)
{
  FVector forward = GetActorForwardVector();
  FVector location = GetActorLocation();
  location += MovementSpeed * DeltaTime * value * forward;
  SetActorLocation(location);

}

void APlayerOvi::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );
  m_lastPosition = GetActorLocation();
	float value = 0;
  FVector up = GetActorUpVector();
  FVector forward = GetActorForwardVector();

	if (m_right && !m_left) {
		m_doJump = false;
		value = 1;

    if (m_state != States::RIGHT)
      Rotate(FVector(0, 0, -90));

		m_state = States::RIGHT;
	}
	else if (m_left && !m_right) {
		m_doJump = false;
		value = 1;
    if (m_state != States::LEFT)
      Rotate(FVector(0, 0, 90));

		m_state = States::LEFT;
	}
	else if (!m_left && !m_right) {
		m_doJump = false;
		value = 0;

    if (m_state == States::RIGHT)
      Rotate(FVector(0, 0, 90));

    if (m_state == States::LEFT)
      Rotate(FVector(0, 0, -90));
 
		m_state = States::STOP;
  }
  else {
    m_doJump =  true;
    if (m_state == States::RIGHT || m_state == States::LEFT)
      value = 1;
  }

  DoMovement(DeltaTime, value);
  DoJump(DeltaTime);
  CalculateGravity(DeltaTime);
  CalculateOrientation();
  CheckCollision();
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

void APlayerOvi::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerOvi::OnStartRight);
	InputComponent->BindAction("MoveRight", IE_Released, this, &APlayerOvi::OnStopRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerOvi::OnStartLeft);
	InputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerOvi::OnStopLeft);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerOvi::TouchStarted);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerOvi::TouchEnd);
}

void APlayerOvi::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location) {

	if (Location.X > m_viewportCenter.X)
		m_right = true;
	if (Location.X < m_viewportCenter.X)
		m_left = true;

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH")));
	}
}

void APlayerOvi::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location) {
	m_right = false;
	m_left = false;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH END")));
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

void APlayerOvi::Rotate(const FVector& rotation) {
  FTransform transform = GetTransform();
  FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(rotation);
  transform.SetRotation(q);
  SetActorTransform(transform);
}
