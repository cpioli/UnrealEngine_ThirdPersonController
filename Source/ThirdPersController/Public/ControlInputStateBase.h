// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "InputStateMachineCharacter.h"
#include "ControlInputStateBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class THIRDPERSCONTROLLER_API UControlInputStateBase : public UObject
{
	GENERATED_BODY()

private:

protected:

public:

	UPROPERTY(EditAnywhere)
	float MaxRunSpeed;

	UPROPERTY(EditAnywhere)
		float MaxSprintSpeed;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnStateEnter(AInputStateMachineCharacter *charRef);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnStateExit(AInputStateMachineCharacter *charRef);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnUpdate(AInputStateMachineCharacter *charRef, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnAnimationUpdate(AInputStateMachineCharacter *charRef, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputAxisLeftAnalogStick(AInputStateMachineCharacter *charRef, float forwardAxisValue, float rightAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputAxisRightAnalogStick(AInputStateMachineCharacter *charRef, float lookUpRate, float turnRate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputJumpButton(AInputStateMachineCharacter *charRef, bool pressedOrReleased);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputSprintButton(AInputStateMachineCharacter *charRef, bool pressedOrReleased);
};
