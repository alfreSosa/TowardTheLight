// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "Tappable.h"


// Sets default values
ATappable::ATappable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATappable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATappable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

