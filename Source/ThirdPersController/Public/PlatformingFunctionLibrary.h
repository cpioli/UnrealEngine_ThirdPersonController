// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputStateMachineCharacter.h"
#include "PlatformingFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class THIRDPERSCONTROLLER_API UPlatformingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	static inline double GetSlopeOfPlaneFromNormal(const FVector& Normal) { return FMath::RadiansToDegrees(FMath::Acos(Normal.Z)); }

	FVector GetWallPosition(AInputStateMachineCharacter *Character);

	//Checks if the body is in range to react to a ledge
	static bool bIsBodyInRangeOfLedgeAtPosition(const UChildActorComponent* Component, 
		const FLedge& CurrentLedge, const FVector& LastUpdateVelocity, 
		const float RelativeLowerBounds, const float RelativeUpperBounds);

	//the parent function that can determine if we can climb ledge.
	//exposes parameters so particular sections of the body can be examined.
	//set to private with no UFUNCTION. UFUNCTIONs will specify sections of the body to check
	static bool bCanClimbLedge(const AInputStateMachineCharacter* Character,
		const UChildActorComponent* Component, 
		const FWallProjectionLocation& WallHeightData,
		const float& RelativeLowerBounds, 
		const float& RelativeUpperBounds);

public:
	static float GetAngle(const FVector & A, const FVector & B);
	
	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bIsPressedAgainstWall(const FVector & forwardVector, const FVector & wallNormalVector, float degreeRange);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bIsClimbableAtPosition(const FVector & velocity, const FVector & position, const FVector & ledgePosition, float heightComparator = 50.0f);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static void HandleStandardInput(APawn* outPawn, const float forwardAxisScale, const float rightAxisScale);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static void DebugLine(const UWorld* InWorld, const FVector& start, const FRotator& direction, const float &distance, 
							   FColor& lineColor,  float duration = 12.0f, float thickness = 5.0f);
	
	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static FVector SnapToLedge(const FWallProjectionLocation& Shoulder, const FLedge& TargetedLedge, const float& zAdjustment);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCollidedWithWall(AInputStateMachineCharacter *Character, const float maxSlopeAngle);

	UFUNCTION(BlueprintPure, Category = "Platforming")
		static bool bNotAgainstWall(AInputStateMachineCharacter* Character);

	UFUNCTION(BlueprintPure, Category = "Platforming", meta=(DisplayName="Wall Is Short"))
		static bool bWallIsShort(AInputStateMachineCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static FVector GetLerpedPosition(const FVector& Begin, const FVector& End, const UCurveVector* Curve, const float T);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCanHangOnLedge(const AInputStateMachineCharacter* Character, float RelativeLowerBounds, float RelativeUpperBounds);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCanMantleLedgeInMidair(const AInputStateMachineCharacter* Character, float RelativeLowerBounds, float RelativeUpperBounds);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCanReachLedgeByFooting(const AInputStateMachineCharacter* Character, float RelativeLowerBounds, float RelativeUpperBounds);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static double GetSlopeOfPlane(const FVector& Normal);

	//**********************************
	// USTRUCT Range's FUNCTIONS
	//**********************************

	UFUNCTION(BlueprintCallable, Category = "Platforming", meta = (DisplayName = "In Range"))
		static bool bInRange(const float& value, const FRange& range);
};
