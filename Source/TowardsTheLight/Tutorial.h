#pragma once

#include "GameFramework/Actor.h"
#include "Tutorial.generated.h"

UCLASS()
class TOWARDSTHELIGHT_API ATutorial : public AActor
{
	GENERATED_BODY()
	
public:	
  //properties
  UPROPERTY()
    UBoxComponent *Trigger;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialText)
    FString Localizador;
  //functions
	ATutorial();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
  void RegisterDelegate();
  void EndPlay(const EEndPlayReason::Type EndPlayReason);
  //Delegates
  UFUNCTION()
    void OnBeginTriggerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
  UFUNCTION()
    void OnTriggerOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
  //properties
  bool m_loaded;
  bool m_enter;
  FString m_text;
	
};
