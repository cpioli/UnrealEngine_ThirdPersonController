// Fill out your copyright notice in the Description page of Project Settings.


#include "InputStateMachineCharacter.h"

// Sets default values
AInputStateMachineCharacter::AInputStateMachineCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInputStateMachineCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInputStateMachineCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInputStateMachineCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

