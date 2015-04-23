// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerOvi.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API APlayerOvi : public APawn
{
	GENERATED_BODY()

public:
	enum States { STOP, RIGHT, LEFT };
	// Sets default values for this pawn's properties
	APlayerOvi();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	virtual void OnBeginOverlap(AActor *other,UPrimitiveComponent *other2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& sweepresult);
	//sets right flag when key is pressed
	UFUNCTION()
		void OnStartRight();
	//clears right flag when key is released
	UFUNCTION()
		void OnStopRight();
	//sets left flag when key is pressed
	UFUNCTION()
		void OnStartLeft();
	//clears left flag when key is released
	UFUNCTION()
		void OnStopLeft();

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	DEPRECATED_FORGAME(4.6, "CapsuleComponent should not be accessed directly, please use GetCapsuleComponent() function instead. CapsuleComponent will soon be private and your code will not compile.")
		UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	DEPRECATED_FORGAME(4.6, "Mesh should not be accessed directly, please use GetMesh() function instead. Mesh will soon be private and your code will not compile.")
		UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA
	DEPRECATED_FORGAME(4.6, "ArrowComponent should not be accessed directly, please use GetArrowComponent() function instead. ArrowComponent will soon be private and your code will not compile.")
		UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float JumpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MaxJumpHeight;
private:
	bool m_right;
	bool m_left;
	bool m_isJumping;
	States m_state;
	float m_limit;
	float m_jumpDistance;
	
};
