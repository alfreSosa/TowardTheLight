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
	m_isJumping = false;
  m_jumpDistance = 0; 
  m_hasLanded = false; 
  m_enabledGravity = true;
	FName *name = new FName("player", EFindName::FNAME_Add);
	FName f = GetFName();
	JumpSpeed = MovementSpeed = 300.0f;
	MaxJumpHeight = 150.0f;
  collisionActor = NULL;
}


void APlayerOvi::ReceiveActorBeginOverlap(AActor * OtherActor)
{
  /*if (OtherActor)
    SetActorLocation(lastPosition);*/
}

void APlayerOvi::ReceiveActorEndOverlap(AActor * OtherActor)
{
  /*if (OtherActor)
    m_hasLanded = false;*/
}

void APlayerOvi::BeginPlay()
{
	Super::BeginPlay();
 
	this->Tags.Add("Player");
	m_limit = FVector::DotProduct(GetActorLocation(), GetActorForwardVector());
	m_limit = abs(m_limit);
  //lastPosition = GetActorLocation();
	
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
  static FName FireTraceIdent = FName(TEXT("WeaponTrace"));
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
   
    //collisionActor = OutTraceResult.Actor.Get();
    FVector loc = GetActorLocation();
    FVector up = GetActorUpVector() * lastPosition;
    loc.X = (FMath::Abs(up.X) <= 0.01) ? loc.X : up.X;
    loc.Y = (FMath::Abs(up.Y) <= 0.01) ? loc.Y : up.Y;
    loc.Z = (FMath::Abs(up.Z) <= 0.01) ? loc.Z : up.Z;
    SetActorLocation(loc);
    m_hasLanded = true;
    m_isJumping = false;
    m_jumpDistance = 0.0f;
  }
  if (collisionUp)
  {
    FVector loc = GetActorLocation();
    FVector up = GetActorUpVector() * lastPosition;
    loc.X = (FMath::Abs(up.X) <= 0.01) ? loc.X : up.X;
    loc.Y = (FMath::Abs(up.Y) <= 0.01) ? loc.Y : up.Y;
    loc.Z = (FMath::Abs(up.Z) <= 0.01) ? loc.Z : up.Z;
    SetActorLocation(loc);
    m_isJumping = false;
    m_jumpDistance = 0.0f;
  }
  if (collisionTop || collisionBottom || collisionMidle)
  {
    FVector loc = GetActorLocation();
    FVector forward = GetActorForwardVector() * lastPosition;
    loc.X = (FMath::Abs(forward.X) <= 0.01) ? loc.X : forward.X;
    loc.Y = (FMath::Abs(forward.Y) <= 0.01) ? loc.Y : forward.Y;
    loc.Z = (FMath::Abs(forward.Z) <= 0.01) ? loc.Z : forward.Z;
    SetActorLocation(loc);
  }

}

void APlayerOvi::CalculateOrientation()
{
  FRotator rot = GetActorRotation();
  FVector forward = GetActorForwardVector();
  FVector up = GetActorUpVector();

  float dotForward = FVector::DotProduct(GetActorLocation(), forward);

  if (dotForward > m_limit && m_state == States::RIGHT)
    rot.Yaw -= 90;
  else if (dotForward > m_limit && m_state == States::LEFT)
    rot.Yaw += 90;

  /*Para rotaciones verticales*/
  float dotUp = FVector::DotProduct(GetActorLocation(), up);

  SetActorRotation(rot);
}

void APlayerOvi::DoJump(float DeltaTime)
{
  FVector up = GetActorUpVector();
  FVector location = GetActorLocation();

  if (m_left && m_right && !m_isJumping){
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
	FRotator rot = GetActorRotation();
	if (m_right && !m_left) {
    value = 1;
		if (m_state != States::RIGHT)
			rot.Yaw -= 90;
		m_state = States::RIGHT;
	}
	else if (m_left && !m_right) {
    value = 1;
		if (m_state != States::LEFT)
			rot.Yaw += 90;
		m_state = States::LEFT;
	}
	else if (!m_left && !m_right) {
		value = 0;
		if (m_state == States::RIGHT)
			rot.Yaw += 90;
		if (m_state == States::LEFT)
			rot.Yaw -= 90;

		m_state = States::STOP;
  }
  else{
    if (m_state == States::RIGHT || m_state == States::LEFT)
      value = 1;
  }
  SetActorRotation(rot);

  CalculateOrientation();
  DoMovement(DeltaTime, value);
  DoJump(DeltaTime);
  CalculateGravity(DeltaTime);
  CheckCollision();
}

void APlayerOvi::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerOvi::OnStartRight);
	InputComponent->BindAction("MoveRight", IE_Released, this, &APlayerOvi::OnStopRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerOvi::OnStartLeft);
	InputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerOvi::OnStopLeft);
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
