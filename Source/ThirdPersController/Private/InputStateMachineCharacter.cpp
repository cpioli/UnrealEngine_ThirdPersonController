// Fill out your copyright notice in the Description page of Project Settings.


#include "InputStateMachineCharacter.h"
#include "InputStateCharacterAnimInstance.h"
#include "ControlInputStateBase.h"
#include "DrawDebugHelpers.h"
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
	FVector ChosenNormal = FVector::ZeroVector;
	if (this->ShoulderToWallHeight.bIsAvailable)
	{
		ChosenNormal = this->ShoulderToWallHeight.Normal;
	}
	else if (this->PelvisToWallHeight.bIsAvailable)
	{
		ChosenNormal = this->PelvisToWallHeight.Normal;
	}
	else if (this->KneeToWallHeight.bIsAvailable)
	{
		ChosenNormal = this->KneeToWallHeight.Normal;
	}
	else if (this->FootToWallHeight.bIsAvailable)
	{
		ChosenNormal = this->FootToWallHeight.Normal;
	}
	else {
		//TODO: raise a warning
	}
	return FRotationMatrix::MakeFromXZ(
		ChosenNormal * -1.0f, 
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
		CharAnimBP->PreviousInputState = CharAnimBP->CurrentInputState;
		CurrentInputState = nullptr;
		PreviousInputState->OnStateExit(this);
	}
	CurrentInputState = *StateRepository.Find(newState);
	CharAnimBP->CurrentInputState = *EnumStateRepository.Find(newState);
	CurrentInputState->OnStateEnter(this);

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

/// <summary>
/// Updates the Anim Instance's state when no change is necessary to the Input Control State
/// Assumes a value for CharAnimBP's CurrentInputState already exists.
/// </summary>
/// <param name="newState"></param>
void AInputStateMachineCharacter::SetCurrentAnimState(TEnumAsByte<EInputState::InputState> newState)
{
	CharAnimBP->PreviousInputState = CharAnimBP->CurrentInputState;
	CharAnimBP->CurrentInputState = newState;
}

// equivalent draw debug line calls from "TraceLineByChannel" Kismet node can be found in this file
// https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Runtime/Engine/Private/KismetTraceUtils.cpp
// look for the function "DrawDebugLineTraceSingle"
void AInputStateMachineCharacter::FindNearbyWalls(const UChildActorComponent* Position, const float Distance, /*EDrawDebugTrace::Type is missing, dunno what to replace it with?*/
	FLinearColor TraceColor, FLinearColor TraceHitColor, FWallProjectionLocation& WallLoc) 
{
	FVector start = Position->GetComponentLocation();
	//gets the position's component rotation, puts it back into the rotaiton matrix format, returns the scaled axis
	//https://answers.unrealengine.com/questions/236461/trying-to-get-the-forward-and-right-vector-from-a.html
	FVector end = start + FRotationMatrix(Position->GetComponentRotation()).GetScaledAxis(EAxis::X) * Distance;

	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.bFindInitialOverlaps = false;
	traceParams.AddIgnoredActor(this);
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, traceParams);
	if (!bIsHit) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString(TEXT("Could not find a wall")), true);
		WallLoc.bIsAvailable = false;
		return;
	}

#if ENABLE_DRAW_DEBUG
	::DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, TraceColor.ToFColor(true), /*bPersistent=*/ false, /*LifeTime=*/ 0.f);
	::DrawDebugLine(GetWorld(), hitResult.ImpactPoint, end, TraceHitColor.ToFColor(true), false, 0.f);
	::DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, /*KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE*/ 16.0f, TraceColor.ToFColor(true), false, 0.f);
#endif
	WallLoc.bIsAvailable = true;
	WallLoc.Location = hitResult.Location;
	WallLoc.Normal = hitResult.Normal;
}

void AInputStateMachineCharacter::FindNearbyLedges(const float DistanceFromCharacter, const float HeightAboveCharacter,
	FLinearColor TraceColor, FLinearColor TraceHitColor)
{
	FVector pos = this->GetMesh()->GetSocketLocation("root");
	FVector end = pos;
	end += FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X) * DistanceFromCharacter;
	FVector start = FVector(end.X, end.Y, end.Z + HeightAboveCharacter); //change this in case player or world orientation changes in a game

	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.bFindInitialOverlaps = false;
	traceParams.AddIgnoredActor(this);
	this->CurrentLedge.bIsAvailable = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, traceParams);
	if (!this->CurrentLedge.bIsAvailable) return;
	this->CurrentLedge.Location = hitResult.Location;
	this->CurrentLedge.HeightFromFloor = this->CurrentLedge.Location.Z - pos.Z;
#if ENABLE_DRAW_DEBUG
	::DrawDebugLine(GetWorld(), start, hitResult.ImpactPoint, TraceColor.ToFColor(true), /*bPersistent=*/ false, /*LifeTime=*/ 0.f);
	::DrawDebugLine(GetWorld(), hitResult.ImpactPoint, end, TraceHitColor.ToFColor(true), /*Persistent=*/ false, /*LifeTime=*/ 0.f);
	::DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, /*KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE*/16.0f, TraceColor.ToFColor(true), false, 0.f);
#endif
	return;

}

void AInputStateMachineCharacter::TraceIKHandToWall(const FName& SocketName, const float& TraceDistance)
{
	bool LeftSocket;
	//TODO: Must not rely on magic strings, should expose these values in the Blueprint incase a mesh uses other names
	if (!SocketName.ToString().Compare("upperarm_lSocket")) //lexicographical comparison, returns 0 if strings match
	{
		LeftSocket = true;
	}
	else if (!SocketName.ToString().Compare("upperarm_rSocket"))
	{
		LeftSocket = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The FName value does not correspond to an upperarm socket"));
		return;
	}
	FVector start = this->GetMesh()->GetSocketLocation(SocketName);
	FVector end = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X);
	end.Normalize();
	end = end * TraceDistance + start;

	FHitResult outHit;
	FCollisionQueryParams traceParams;
	traceParams.bFindInitialOverlaps = false;
	traceParams.AddIgnoredActor(this);
	if (LeftSocket)
	{
		this->bLeftHandAgainstWall = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, traceParams);
		if (!this->bLeftHandAgainstWall) return;
		this->IKLeftHandPosition = outHit.Location;
	}
	else {
		this->bRightHandAgainstWall = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, traceParams);
		if (!this->bRightHandAgainstWall) return;
		this->IKRightHandPosition = outHit.Location;
	}
	return;
}