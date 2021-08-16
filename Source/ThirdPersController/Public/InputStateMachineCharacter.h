// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputStateMachineCharacter.generated.h"

USTRUCT(BlueprintType, DisplayName="WallProjection")
struct FWallProjectionLocation {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAvailable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Normal;
};

USTRUCT(BlueprintType, DisplayName="Ledge")
struct FLedge {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAvailable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HeightFromFloor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;
};

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State Machine")
	TMap<TSubclassOf<UControlInputStateBase>, UControlInputStateBase*> StateRepository;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State Machine")
		TMap<TSubclassOf<UControlInputStateBase>, TEnumAsByte<EInputState::InputState>> EnumStateRepository;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State Machine")
		UControlInputStateBase *CurrentInputState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State Machine")
		UControlInputStateBase *PreviousInputState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		FVector IKLeftHandPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		FVector IKRightHandPosition;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		bool bLeftHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		bool bRightHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FWallProjectionLocation KneeToWallHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FWallProjectionLocation PelvisToWallHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FWallProjectionLocation ShoulderToWallHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLedge CurrentLedge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSet<AActor*> OverlappingObjects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanTrace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CurrentHeightFromFloor;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		float GetHeightFromFloor(float downwardTraceDistance = 900.0f);

	UFUNCTION(BlueprintCallable)
		FRotator GetAlignmentToWall();

	//TODO: check and determine if I need to use a pointer to UControlInputStateBase instead of
	//      TSubclassOf<UControlInputStateBase>
	UFUNCTION(BlueprintCallable)
		void SetCurrentState(TSubclassOf<UControlInputStateBase> newState);

	UFUNCTION(BlueprintCallable)
		void SetCurrentAnimState(TEnumAsByte<EInputState::InputState> newState);
};
