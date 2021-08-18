// Fill out your copyright notice in the Description page of Project Settings.


#include "InputStateMachineCharacter.h"
#include "ControlInputStateBase.h"
#include "Components/CapsuleComponent.h"

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

float AInputStateMachineCharacter::GetHeightFromFloor(float downwardTraceDistance)
{
	FVector location = this->GetActorLocation();
	FHitResult hitResult;

	FVector start = FVector(location.X, location.Y, location.Z - this->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	FVector end = FVector(location.X, location.Y, start.Z - downwardTraceDistance);

	FCollisionQueryParams traceParams;
	traceParams.bFindInitialOverlaps = false;
	traceParams.AddIgnoredActor(this);
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, traceParams);
	if (bIsHit) return hitResult.Distance;
	else return -1.0f;
}


// Original Blueprint function name was "AlignToWall"
// Note: found MakeFromXZ source here
//https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.inl
FRotator AInputStateMachineCharacter::GetAlignmentToWall()
{
	return FRotationMatrix::MakeFromXZ(
		this->ShoulderToWallHeight.Normal * -1.0f, 
		this->GetCapsuleComponent()->GetUpVector()).Rotator();

}

void AInputStateMachineCharacter::SetCurrentState(TSubclassOf<UControlInputStateBase> newState)
{
	if (newState == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("New State has not been specified!"));
		//FDebug::DumpStackTraceToLog();
		return;
	}
	if (!StateRepository.Contains(newState)) return;
	if (CurrentInputState != nullptr)
	{
		if (this->CurrentInputState->GetClass() == newState->GetClass())
			return;
		PreviousInputState = CurrentInputState;
		PreviousInputState->OnStateExit(this);
	}
	CurrentInputState = *StateRepository.Find(newState);
	CurrentInputState->OnStateEnter(this);

	UE_LOG(LogTemp, Warning, TEXT("Does this work outside the if statement?"));
	if (GEngine)
	{
		FString statesChanged = FString(TEXT(""));
		if (PreviousInputState != nullptr)
		{
			statesChanged.Append(PreviousInputState->GetFName().ToString());
			statesChanged.Append("->");
		}
		statesChanged.Append(CurrentInputState->GetFName().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, statesChanged, true, FVector2D(1.f, 1.f));
	}
}

void AInputStateMachineCharacter::SetCurrentAnimState(TEnumAsByte<EInputState::InputState> newState)
{

}