// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformingFunctionLibrary.h"
#include "InputStateMachineCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

/*
Find out if there is a wall nearby us
UNUSED
*/
FVector UPlatformingFunctionLibrary::GetWallPosition(AInputStateMachineCharacter* character)
{
	FVector pos = character->GetMesh()->GetSocketLocation("root");
	FVector start(pos.X, pos.Y, pos.Z + 1.0f);
	float heightFactor = 0.0f;
	//if(character.GetVelocity().Z < -200.0f && character.GetCharacterMovement().GetMovementMode() == 
	return FVector(0.0f, 0.0f, 0.0f);
}


bool UPlatformingFunctionLibrary::bIsBodyInRangeOfLedgeAtPosition(const UChildActorComponent* Component,
	const FLedge& CurrentLedge, const FVector& LastUpdateVelocity,
	const float RelativeLowerBounds, const float RelativeUpperBounds)
{
	float RelativeLedgeToComponentDistance = CurrentLedge.Location.Z - Component->GetComponentLocation().Z;
	if (LastUpdateVelocity.Z < 0.0f) {
		return -10.0f <= RelativeLedgeToComponentDistance
			&& 10.0f >= RelativeLedgeToComponentDistance;
	}
	else {
		return RelativeLowerBounds <= RelativeLedgeToComponentDistance
			&& RelativeUpperBounds >= RelativeLedgeToComponentDistance;
	}
}

bool UPlatformingFunctionLibrary::bCanClimbLedge(const AInputStateMachineCharacter* Char,
	const UChildActorComponent* Component,
	const FWallProjectionLocation& WallHeightData,
	const float& RelativeLowerBounds,
	const float& RelativeUpperBounds)
{
	if (!WallHeightData.bIsAvailable) return false;

	FVector forward = FRotationMatrix(Char->GetActorRotation()).GetScaledAxis(EAxis::X);
	FVector lastUpdateVelocity = Char->GetCharacterMovement()->GetLastUpdateVelocity();

	return bIsPressedAgainstWall(forward, WallHeightData.Normal, /*DegreeRange*/10.0f)
		&& bIsBodyInRangeOfLedgeAtPosition(Component, Char->CurrentLedge, lastUpdateVelocity, RelativeLowerBounds, RelativeUpperBounds)
		&& lastUpdateVelocity.Size2D() >= 0.0f;
}

/*
generic function to retrieve an angle from two vectors
*/

float UPlatformingFunctionLibrary::GetAngle(const FVector & A, const FVector & B)
{
	float dotProductValue = FVector::DotProduct(A, B);
	dotProductValue /= (A.Size() * B.Size());
	return acos(dotProductValue);
}



bool UPlatformingFunctionLibrary::bIsPressedAgainstWall(const FVector & forwardVector, const FVector & wallNormalVector, float degreeRange)
{
	FVector vectorB = wallNormalVector * -1.0f;
	float degree = GetAngle(forwardVector, wallNormalVector);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("Degree: %02d"), degree), true, FVector2D(1.f, 1.f));
	return degree < degreeRange;
}


//currently unused, meant to replace MidairInputState.InRangeOfLedge(CharRef)
bool UPlatformingFunctionLibrary::bIsClimbableAtPosition(const FVector & velocity, const FVector & position, const FVector & ledgePosition, float heightComparator)
{
	float ledgeToShoulderDistance = ledgePosition.Z - position.Z;
	
	//if the player character is falling, restrict detection to a range of 20 units
	if (velocity.Z > 0.0f) {
		return ledgeToShoulderDistance >= 10.0f && ledgeToShoulderDistance <= 10.0f;
	}
	//if the player character is not falling, use this
	else
	{
		return ledgeToShoulderDistance < heightComparator;
	}
}

void UPlatformingFunctionLibrary::HandleStandardInput(APawn* outPawn, const float forwardAxisScale, const float rightAxisScale)
{
	if (outPawn->Controller == nullptr) return;

	const FRotator pawnRotator = outPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, pawnRotator.Yaw, 0);

	if (forwardAxisScale != 0.0f) {
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		outPawn->AddMovementInput(Direction, forwardAxisScale);
	}

	if (rightAxisScale != 0.0f) {
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		outPawn->AddMovementInput(Direction, rightAxisScale);
	}
}


//Lookup Quat.h https://github.com/EpicGames/UnrealEngine/blob/c3caf7b6bf12ae4c8e09b606f10a09776b4d1f38/Engine/Source/Runtime/Core/Public/Math/Quat.h
void UPlatformingFunctionLibrary::DebugLine(const UWorld* InWorld, const FVector& start, const FRotator& direction, const float& distance, FColor &lineColor, float duration, float thickness)
{
	FVector _start = start;
	FVector _end = _start + direction.Quaternion().GetForwardVector() * distance;
	DrawDebugLine(InWorld, _start, _end, lineColor, false, duration, 0U, thickness);
}

FVector UPlatformingFunctionLibrary::SnapToLedge(const FWallProjectionLocation& Shoulder, const FLedge& TargetedLedge)
{
	FVector sol;
	sol.X = Shoulder.Location.X + Shoulder.Normal.X * 22.0f; //22.0 is a magic number representing the radius of the capsule collider.
	sol.Y = Shoulder.Location.Y + Shoulder.Normal.Y * 22.0f;
	sol.Z = TargetedLedge.Location.Z - 98.0f; //Another magic number representing where the midpoint of the capsule collider should be positioned

	return sol;
}

bool UPlatformingFunctionLibrary::bCollidedWithWall(AInputStateMachineCharacter* Character)
{
	return Character->ShoulderToWallHeight.bIsAvailable
		|| Character->PelvisToWallHeight.bIsAvailable
		|| Character->KneeToWallHeight.bIsAvailable;
}

//not including foot wall position here because that could just be a step
bool UPlatformingFunctionLibrary::bNotAgainstWall(AInputStateMachineCharacter* Character)
{
	return !Character->ShoulderToWallHeight.bIsAvailable
		&& !Character->PelvisToWallHeight.bIsAvailable
		&& !Character->KneeToWallHeight.bIsAvailable;
}

bool UPlatformingFunctionLibrary::bWallIsShort(AInputStateMachineCharacter* Character)
{
	return !Character->ShoulderToWallHeight.bIsAvailable
		&& (Character->PelvisToWallHeight.bIsAvailable || Character->KneeToWallHeight.bIsAvailable);

}
//TODO: refactor this function so it's generalized for any child component position, not just shoulders
bool UPlatformingFunctionLibrary::bUpperBodyInRangeOfLedge(const FLedge& CurrentLedge, const UChildActorComponent* Position, const FVector& LastUpdateVelocity)
{
	float LedgeToShoulderDistance = CurrentLedge.Location.Z - Position->GetComponentLocation().Z;
	if (LastUpdateVelocity.Z > 0.f) { //if we are moving up in the elevation direction
		return -10.f <= LedgeToShoulderDistance && 10.f >= LedgeToShoulderDistance;
	}
	else {
		return -15.0f <= LedgeToShoulderDistance && LedgeToShoulderDistance <= 30.0f;
	}
}

bool UPlatformingFunctionLibrary::bLowerBodyInRangeOfLedge(const UChildActorComponent* ComponentLocation, const FLedge& CurrentLedge, const FVector& LastUpdateVelocity, const float maxDistance)
{
	float LedgeToChildActorDistance = CurrentLedge.Location.Z - ComponentLocation->GetComponentLocation().Z;
	if (LastUpdateVelocity.Z > 0.0f) {
		return -10.f <= LedgeToChildActorDistance && 10.f >= LedgeToChildActorDistance;
	}
	return FMath::Abs(LedgeToChildActorDistance) <= maxDistance;
}

//VectorLengthXY node is called as static float VSizeXY(FVector A) in C++
//The function is just "Size2D()"
//Also: needed to include "GameFrameworks/CharacterMovementComponent.h" to retrieve lastUpdateVelocity
bool UPlatformingFunctionLibrary::bCanClimbLedgeUpperBody(const AInputStateMachineCharacter* Character, const UChildActorComponent* Position)
{
	if (!Character->ShoulderToWallHeight.bIsAvailable) return false;

	FVector forward = FRotationMatrix(Character->GetActorRotation()).GetScaledAxis(EAxis::X);

	FVector lastUpdateVelocity = Character->GetCharacterMovement()->GetLastUpdateVelocity();
	
	return bIsPressedAgainstWall(forward, Character->ShoulderToWallHeight.Normal, /*DegreeRange*/10.0f)
		&& bUpperBodyInRangeOfLedge(Character->CurrentLedge, Position, lastUpdateVelocity)
		&& lastUpdateVelocity.Size2D() >= 0.0f;
}

bool UPlatformingFunctionLibrary::bCanClimbLedgeLowerBody(const AInputStateMachineCharacter* Character, const UChildActorComponent* PelvicPos, const UChildActorComponent* KneePos)
{
	const UChildActorComponent* UsablePosition = nullptr;
	FVector normal = FVector::ZeroVector;
	bool usingKnee = false;
	if (Character->PelvisToWallHeight.bIsAvailable) {
		UsablePosition = PelvicPos;
		usingKnee = false;
		normal = Character->PelvisToWallHeight.Normal;
	}
	else if (Character->KneeToWallHeight.bIsAvailable) {
		UsablePosition = KneePos;
		usingKnee = true;
		normal = Character->KneeToWallHeight.Normal;
	}
	else {
		return false;
	}

	FVector forward = FRotationMatrix(Character->GetActorRotation()).GetScaledAxis(EAxis::X);
	FVector lastUpdateVelocity = Character->GetCharacterMovement()->GetLastUpdateVelocity();

	return bIsPressedAgainstWall(forward, normal, 10.0f)
		&& bLowerBodyInRangeOfLedge(UsablePosition, Character->CurrentLedge, lastUpdateVelocity, 30.0f)
		&& lastUpdateVelocity.Size2D() >= 0.0f;
}

/*
Calculates the position of an object based on its start and end values
using a UCurveVector object as the Interpolation type
*/

FVector UPlatformingFunctionLibrary::GetLerpedPosition(const FVector& Begin, const FVector& End, const UCurveVector* Curve, const float T)
{
	FVector sol = FVector::ZeroVector;
	if (Curve == nullptr) return sol;
	float Tstart, Tend;
	Curve->GetTimeRange(Tstart, Tend);
	if (T < Tstart || T > Tend) return sol; //TODO: add a UELOG message

	sol.X = Begin.X + (End.X - Begin.X) * Curve->FloatCurves[0].Eval(T);
	sol.Y = Begin.Y + (End.Y - Begin.Y) * Curve->FloatCurves[1].Eval(T);
	sol.Z = Begin.Z + (End.Z - Begin.Z) * Curve->FloatCurves[2].Eval(T);

	return sol;
}

bool UPlatformingFunctionLibrary::bCanHangOnLedge(const AInputStateMachineCharacter* Char, float RelativeLowerBounds, float RelativeUpperBounds)
{
	return bCanClimbLedge(Char, Char->ShoulderPositionComponent, Char->ShoulderToWallHeight, RelativeLowerBounds, RelativeUpperBounds);
}


bool UPlatformingFunctionLibrary::bCanMantleLedgeInMidair(const AInputStateMachineCharacter* Char, float RelativeLowerBounds, float RelativeUpperBounds)
{
	return bCanClimbLedge(Char, Char->PelvisPositionComponent, Char->PelvisToWallHeight, RelativeLowerBounds, RelativeUpperBounds);
}