// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StaticEnemy.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AStaticEnemy : public AActor
{
	GENERATED_BODY()

  TScriptDelegate<FWeakObjectPtr> m_delegate;

public:
	// Sets default values for this pawn's properties
	AStaticEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
  UPROPERTY(EditAnywhere)
    UStaticMeshComponent* OurVisibleComponent;
	
  void RegisterDelegate();
  UFUNCTION()
    void OnCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
};
