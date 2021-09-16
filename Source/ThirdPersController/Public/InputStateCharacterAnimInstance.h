// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InputStateMachineCharacter.h"
#include "InputStateCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSCONTROLLER_API UInputStateCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsInAir;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool LeftHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool RightHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EInputState::InputState> PreviousInputState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EInputState::InputState> CurrentInputState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HeightFromGround;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector IKLeftHandEffector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector IKRightHandEffector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AInputStateMachineCharacter* CharRef;
};
