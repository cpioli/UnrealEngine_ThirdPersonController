// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ControlInputStateBase.h"
#include "InputStateMachineCharacter.generated.h"



UENUM(BlueprintType)
namespace EInputState
{
	enum InputState
	{
		IS_Grounded		UMETA(DisplayName = "Grounded"),
		IS_Midair		UMETA(DisplayName = "Midair"),
		IS_Sprinting	UMETA(DisplayName = "Sprinting"),
		IS_Hanging		UMETA(DisplayName = "Hanging"),
		IS_Mantling		UMETA(DisplayName = "Mantling"),
		IS_AgainstWall	UMETA(DisplayName = "Against Wall")
	};
}

//forward declarations to handle circular dependency
//between this class and ControlInputStateBase.h
class UControlInputStateBase;

UCLASS()
class THIRDPERSCONTROLLER_API AInputStateMachineCharacter : public ACharacter
{
	GENERATED_BODY()

private:

public:
	// Sets default values for this character's properties
	AInputStateMachineCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSprintButtonPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsJumpButtonPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
	TMap<TSubclassOf<UControlInputStateBase>, UControlInputStateBase*> StateRepository;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		TMap<TSubclassOf<UControlInputStateBase>, TEnumAsByte<EInputState::InputState>> EnumStateRepository;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
