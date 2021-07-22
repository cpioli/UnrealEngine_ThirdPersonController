// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformingFunctionLibrary.h"
#include "InputStateMachineCharacter.h"

/*
generic function to retrieve an angle from two vectors
*/

float UPlatformingFunctionLibrary::GetAngle(const FVector & A, const FVector & B)
{
	float dotProductValue = FVector::DotProduct(A, B);
	dotProductValue /= (A.Size() * B.Size());
	return acos(dotProductValue);
}

	/*
	Find out if there is a wall nearby us
	*/
FVector UPlatformingFunctionLibrary::GetWallPosition(AInputStateMachineCharacter *character)
{
	FVector pos = character->GetMesh()->GetSocketLocation("root");
	FVector start(pos.X, pos.Y, pos.Z + 1.0f);
	float heightFactor = 0.0f;
	//if(character.GetVelocity().Z < -200.0f && character.GetCharacterMovement().GetMovementMode() == 
	return FVector(0.0f, 0.0f, 0.0f);
}

bool UPlatformingFunctionLibrary::bIsPressedAgainstWall(const FVector & forwardVector, const FVector & wallNormalVector, float degreeRange)
{
	FVector vectorB = wallNormalVector * -1.0f;
	float degree = GetAngle(forwardVector, wallNormalVector);
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