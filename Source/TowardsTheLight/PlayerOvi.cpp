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

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ACharacter::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);

	static FName CollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetCollisionProfileName(CollisionProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->bShouldUpdatePhysicsVolume = true;
	CapsuleComponent->bCheckAsyncSceneOnMove = false;
	CapsuleComponent->bCanEverAffectNavigation = false;
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

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacter::MeshComponentName);
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
		Mesh->bGenerateOverlapEvents = false;
		Mesh->bCanEverAffectNavigation = false;
	}

	m_state = States::STOP;
	m_limit = 0;
	m_isJumping = false;
	m_jumpDistance = 0;

	FName *name = new FName("player", EFindName::FNAME_Add);
	FName f = GetFName();
	JumpSpeed = MovementSpeed = 300.0f;
	MaxJumpHeight = 150.0f;
	CapsuleComponent->bGenerateOverlapEvents = true;
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerOvi::OnBeginOverlap);
}

void  APlayerOvi::OnBeginOverlap(AActor *other, UPrimitiveComponent *other2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepresult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable")));
	}
}
void APlayerOvi::BeginPlay()
{
	Super::BeginPlay();
	this->Tags.Add("Player");

	m_limit = FVector::DotProduct(GetActorLocation(), GetActorForwardVector());
	m_limit = abs(m_limit);
	
}

void APlayerOvi::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	float value = 1;
	FRotator rot = GetActorRotation();
	if (m_right && !m_left) {
		if (m_state != States::RIGHT)
			rot.Yaw -= 90;
		m_state = States::RIGHT;
	}
	else if (m_left && !m_right) {
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
	SetActorRotation(rot);
	FVector forward = GetActorForwardVector();
	FVector up = GetActorUpVector();

	FVector location = GetActorLocation();
	location += MovementSpeed * DeltaTime * value * forward;

	if (m_left && m_right && !m_isJumping)
		m_isJumping = true;

	if (m_isJumping) {
		m_jumpDistance += JumpSpeed * DeltaTime;
		if (m_jumpDistance < MaxJumpHeight)
			location += JumpSpeed * 2 * DeltaTime * up;
	}
	
	location -= JumpSpeed * DeltaTime * up;
	

	float dotForward = FVector::DotProduct(GetActorLocation(), forward);

	if (dotForward > m_limit && m_state == States::RIGHT)
		rot.Yaw -= 90;
	else if (dotForward > m_limit && m_state == States::LEFT)
		rot.Yaw += 90;

	SetActorRotation(rot);
	SetActorLocation(location);
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
