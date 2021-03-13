// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "ControlInputStateBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class THIRDPERSCONTROLLER_API UControlInputStateBase : public UObject
{
	GENERATED_BODY()

private:

protected:

public:


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnStateEnter(ACharacter *charRef);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnStateExit(ACharacter *charRef);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnUpdate(ACharacter *charRef, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnAnimationUpdate(ACharacter *charRef, float deltaTime);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputAxisLeftAnalogStick(ACharacter *charRef, float forwardAxisValue, float rightAxisValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputAxisRightAnalogStick(ACharacter *charRef, float lookUpRate, float turnRate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputJumpButton(ACharacter *charRef, bool pressedOrReleased);
};
