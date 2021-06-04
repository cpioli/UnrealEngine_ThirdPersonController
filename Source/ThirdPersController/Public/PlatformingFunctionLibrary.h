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
		static bool PressedAgainstWall(const FVector & forwardVector, const FVector & wallNormalVector, float degreeRange);

	UFUNCTION(BlueprintCallable, Category = "Platforming")
		static bool IsClimbableAtPosition(const FVector & velocity, const FVector & position, const FVector & ledgePosition, float heightComparator = 50.0f);
	
};
