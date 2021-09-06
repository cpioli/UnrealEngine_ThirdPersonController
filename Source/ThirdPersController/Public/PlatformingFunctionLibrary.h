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
	FVector GetWallPosition(AInputStateMachineCharacter *character);

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
		static FVector SnapToLedge(const FWallProjectionLocation& Shoulder, const FLedge& TargetedLedge);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCollidedWithWall(AInputStateMachineCharacter *Character);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bNotAgainstWall(AInputStateMachineCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bWallIsShort(AInputStateMachineCharacter* Character);

	//Need to separate the UChildActorComponent object because C++ would not be aware of Components added in the Blueprint phase.
	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bUpperBodyInRangeOfLedge(const FLedge &CurrentLedge, const UChildActorComponent *Position, const FVector &LastUpdateVelocity);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bLowerBodyInRangeOfLedge(const UChildActorComponent* ComponentLocation, const FLedge& CurrentLedge, const FVector& LastUpdateVelocity, const float maxDistance);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCanClimbLedge(const AInputStateMachineCharacter *Character, const UChildActorComponent *Position);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool bCanClimbLedgeLowerBody(const AInputStateMachineCharacter* Character, const UChildActorComponent* PelvicPos, const UChildActorComponent* KneePos);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static FVector GetLerpedPosition(const FVector& Begin, const FVector& End, const UCurveVector* Curve, const float T);
};
