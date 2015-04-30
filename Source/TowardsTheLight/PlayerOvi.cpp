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

APlayerOvi::APlayerOvi()
{
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
	m_isJumping = false;
  m_doJump = false;
  m_hasLanded = false; 
  m_enabledGravity = true;
	FName *name = new FName("player", EFindName::FNAME_Add);
	FName f = GetFName();
	JumpSpeed = MovementSpeed = 300.0f;
	MaxJumpHeight = 150.0f;
  collisionActor = NULL;
  m_rotation = FVector::ZeroVector;
}


void APlayerOvi::ReceiveActorBeginOverlap(AActor * OtherActor)
{

}

void APlayerOvi::ReceiveActorEndOverlap(AActor * OtherActor)
{

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
  FVector StartTraceTop = GetActorLocation() + GetActorUpVector() * 86.0f;
  FVector StartTraceBottom = GetActorLocation() - GetActorUpVector() * 80.0f;

  // Calculate endpoint of trace  
  const FVector EndTraceDown = StartTrace - GetActorUpVector() * 88.0f; //half height capsule
  const FVector EndTraceUp = StartTrace + GetActorUpVector() * 88.0f; //half height capsule
  //horizontal
  const FVector EndTraceTop = StartTraceTop + GetActorForwardVector() * 32.0f; //radious capsule
  const FVector EndTraceBottom = StartTraceBottom + GetActorForwardVector() * 32.0f; //radious capsule
  const FVector EndTraceMidle = StartTrace + GetActorForwardVector() * 32.0f; //radious capsule

  // Setup the trace query  
  static FName FireTraceIdent = FName(TEXT("ColliderTrace"));
  FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
  TraceParams.bTraceAsyncScene = true;

  // Perform the trace  
  bool collisionDown = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceDown, COLLISION_PLAYER, TraceParams);
  bool collisionUp = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceUp, COLLISION_PLAYER, TraceParams);

  bool collisionTop = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceTop, EndTraceTop, COLLISION_PLAYER, TraceParams);
  bool collisionBottom = GetWorld()->LineTraceSingle(OutTraceResult, StartTraceBottom, EndTraceBottom, COLLISION_PLAYER, TraceParams);
  bool collisionMidle = GetWorld()->LineTraceSingle(OutTraceResult, StartTrace, EndTraceMidle, COLLISION_PLAYER, TraceParams);

  if (collisionDown)
  {
    FVector loc = GetActorLocation();
    FVector absUp = FVector::ZeroVector;
    FVector up = GetActorUpVector();

    absUp.X = (FMath::Abs(up.X) <= 0.01) ? 0 : 1;
    absUp.Y = (FMath::Abs(up.Y) <= 0.01) ? 0 : 1;
    absUp.Z = (FMath::Abs(up.Z) <= 0.01) ? 0 : 1;

    FVector upPosition = absUp * lastPosition;

    loc.X = (FMath::Abs(upPosition.X) <= 0.01) ? loc.X : upPosition.X;
    loc.Y = (FMath::Abs(upPosition.Y) <= 0.01) ? loc.Y : upPosition.Y;
    loc.Z = (FMath::Abs(upPosition.Z) <= 0.01) ? loc.Z : upPosition.Z;
    SetActorLocation(loc);

    m_hasLanded = true;
    m_isJumping = false;
    m_jumpDistance = 0.0f;
  }
  if (collisionUp)
  {
    FVector loc = GetActorLocation();

    FVector absUp = FVector::ZeroVector;
    FVector up = GetActorUpVector();

    absUp.X = (FMath::Abs(up.X) <= 0.01) ? 0 : 1;
    absUp.Y = (FMath::Abs(up.Y) <= 0.01) ? 0 : 1;
    absUp.Z = (FMath::Abs(up.Z) <= 0.01) ? 0 : 1;

    FVector upPosition = absUp * lastPosition;


    loc.X = (FMath::Abs(upPosition.X) <= 0.01) ? loc.X : upPosition.X;
    loc.Y = (FMath::Abs(upPosition.Y) <= 0.01) ? loc.Y : upPosition.Y;
    loc.Z = (FMath::Abs(upPosition.Z) <= 0.01) ? loc.Z : upPosition.Z;
    SetActorLocation(loc);
    m_isJumping = false;
    m_jumpDistance = 0.0f;
  }
  if (collisionTop || collisionBottom || collisionMidle)
  {
    FVector loc = GetActorLocation();
    FVector absForward = FVector::ZeroVector;
    FVector forward = GetActorForwardVector();

    absForward.X = (FMath::Abs(forward.X) <= 0.01) ? 0 : 1;
    absForward.Y = (FMath::Abs(forward.Y) <= 0.01) ? 0 : 1;
    absForward.Z = (FMath::Abs(forward.Z) <= 0.01) ? 0 : 1;

    FVector forPosition = absForward * lastPosition;

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
  FTransform transform = GetTransform();

  float dotForward = FVector::DotProduct(GetActorLocation(), forward);

  if (dotForward > m_limit && m_state == States::RIGHT) {
      //m_rotation -= up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, -90));
      transform.SetRotation(q);
      SetActorTransform(transform);
      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("RIGHT TRANSITION")));
      }
  }
  
  if (dotForward > m_limit && m_state == States::LEFT) {
      //m_rotation += up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 90));
      transform.SetRotation(q);
      SetActorTransform(transform);
      if (GEngine)
      {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("LEFT TRANSITION")));
      }
  }

  /*Para rotaciones verticales*/
  float dotUp = FVector::DotProduct(GetActorLocation(), up);
  float val = 0;

  if (dotUp > m_limit || dotUp < -m_limit) {
    bool toUp = dotUp > m_limit;	

    val = (toUp) ? 90 : -90;
    //m_rotation.Y += val;
    if (m_state == States::STOP) {
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, val, 0));
      transform.SetRotation(q);
    }
    else if (m_state == States::RIGHT) {
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(-val, 0, 0));
      transform.SetRotation(q);
    }
    else if (m_state == States::LEFT) {
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(val, 0, 0));
      transform.SetRotation(q);
    }
    SetActorTransform(transform);
    if (GEngine)
    {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UP TRANSITION")));
    }
  }

  AjustPosition();

}

void APlayerOvi::DoJump(float DeltaTime)
{
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  if (m_doJump && !m_isJumping){
    m_isJumping = true;
    m_jumpDistance = 0.0f;
    m_hasLanded = false;
  }

  if (m_isJumping) {
    m_jumpDistance += JumpSpeed * DeltaTime;
    if (m_jumpDistance < MaxJumpHeight){
      location += JumpSpeed * 2 * DeltaTime * up;
    }
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
  lastPosition = GetActorLocation();
	float value = 0;
  FVector up = GetActorUpVector();
  FVector forward = GetActorForwardVector();
  FTransform transform = GetTransform();
	if (m_right && !m_left) {
		m_doJump = false;
		value = 1;
    if (m_state != States::RIGHT)
    {
      //m_rotation -= up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0,0,-90));
      transform.SetRotation(q);
      SetActorTransform(transform);
    }

		m_state = States::RIGHT;
	}
	else if (m_left && !m_right) {
		m_doJump = false;
		value = 1;
    if (m_state != States::LEFT) {
      //m_rotation += up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 90));
      transform.SetRotation(q);
      SetActorTransform(transform);
    }
		m_state = States::LEFT;
	}
	else if (!m_left && !m_right) {
		m_doJump = false;
		value = 0;

    if (m_state == States::RIGHT){
      //m_rotation += up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 90));
      transform.SetRotation(q);
      SetActorTransform(transform);
    }

    if (m_state == States::LEFT){
      //m_rotation -= up * 90;
      FQuat q = transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, -90));
      transform.SetRotation(q);
      SetActorTransform(transform);
    }
 
		m_state = States::STOP;
  }
  else{
    m_doJump =  true;
    if (m_state == States::RIGHT || m_state == States::LEFT)
      value = 1;
  }

  //SetActorRelativeRotation(FRotator::MakeFromEuler(m_rotation));

  DoMovement(DeltaTime, value);
  DoJump(DeltaTime);
  CalculateGravity(DeltaTime);
  CalculateOrientation();
  CheckCollision();
  //AjustPosition();
}

void APlayerOvi::AjustPosition(){
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

void APlayerOvi::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// only fire for first finger down
	if (Location.X > m_viewportCenter.X)
		m_right = true;
	if (Location.X < m_viewportCenter.X)
		m_left = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TOUCH")));
	}
}

void APlayerOvi::TouchEnd(const ETouchIndex::Type FingerIndex, const FVector Location)
{

	m_right = false;
	m_left = false;
	if (GEngine)
	{
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
