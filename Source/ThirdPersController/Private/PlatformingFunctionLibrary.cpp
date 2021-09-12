// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformingFunctionLibrary.h"
#include "InputStateMachineCharacter.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

//PRIVATE FUNCTIONS

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
		return -15.0f <= RelativeLedgeToComponentDistance
			&& 15.0f >= RelativeLedgeToComponentDistance;
	}
	else {
		return RelativeLowerBounds <= RelativeLedgeToComponentDistance
			&& RelativeUpperBounds >= RelativeLedgeToComponentDistance;
	}
}

bool UPlatformingFunctionLibrary::bCanClimbLedge(
	const AInputStateMachineCharacter* Char,
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

//PUBLIC FUNCTIONS

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

FVector UPlatformingFunctionLibrary::SnapToLedge(const FWallProjectionLocation& Shoulder, const FLedge& TargetedLedge, const float& zAdjustment)
{
	FVector sol;
	sol.X = Shoulder.Location.X + Shoulder.Normal.X * 22.0f; //22.0 is a magic number representing the radius of the capsule collider.
	sol.Y = Shoulder.Location.Y + Shoulder.Normal.Y * 22.0f;
	sol.Z = TargetedLedge.Location.Z - zAdjustment + 10.0f; //Another magic number representing where the midpoint of the capsule collider should be positioned

	return sol;
}

bool UPlatformingFunctionLibrary::bCollidedWithWall(AInputStateMachineCharacter* Character, const float MaxSlopeAngle)
{
	bool isWalkable = false;;
	if (Character->FootToWallHeight.bIsAvailable)
	{
		FVector Normal = Character->FootToWallHeight.Normal;

		double angle = FMath::RadiansToDegrees(FMath::Acos(Normal.Z));
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("Floor slope is %f"), angle, true));
		isWalkable = angle < MaxSlopeAngle;
		if (isWalkable) return false;
		
		//next check: if it's just a step (ie. not a high wall height
	}
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

bool UPlatformingFunctionLibrary::bCanReachLedgeByFooting(const AInputStateMachineCharacter* Char, float RelativeLowerBounds, float RelativeUpperBounds)
{
	return bCanClimbLedge(Char, Char->FootPositionComponent, Char->FootToWallHeight, RelativeLowerBounds, RelativeUpperBounds);
}

double UPlatformingFunctionLibrary::GetSlopeOfPlane(const FVector& Normal)
{
	return GetSlopeOfPlaneFromNormal(Normal);
}

bool UPlatformingFunctionLibrary::bInRange(const float& value, const FRange& range) {
	return value >= range.Minimum && value < range.Maximum;
}