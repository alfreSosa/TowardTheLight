// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StaticEnemy.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API AStaticEnemy : public AActor
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
    UStaticMeshComponent* OurVisibleComponent;
  UPROPERTY(EditAnywhere)
    float TimeToKill;

	AStaticEnemy();
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
  void RegisterDelegate();
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  virtual void RestoreInitialState();
  UFUNCTION()
    void OnCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  UFUNCTION()
    void OnEndCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
  
private:
  TScriptDelegate<FWeakObjectPtr> m_delegateEnd;
  TScriptDelegate<FWeakObjectPtr> m_delegate;
  bool m_touchingPlayer;
  float m_elapsedTime;
};
