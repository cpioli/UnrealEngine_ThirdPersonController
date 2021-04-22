// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
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
	float maxRunSpeed;

	UPROPERTY(EditAnywhere)
		float maxSprintSpeed;

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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void InputSprintButton(ACharacter *charRef, bool pressedOrReleased);
};
