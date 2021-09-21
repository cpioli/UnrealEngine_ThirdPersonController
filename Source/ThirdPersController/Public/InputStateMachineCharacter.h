// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "InputStateCharacterAnimInstance.h"
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

USTRUCT(BlueprintType, DisplayName="Range")
struct FRange {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Minimum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Maximum;
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

UENUM(BlueprintType)
namespace EWallCollision
{
	enum WallCollisionType
	{
		WCT_Shoulders	UMETA(DisplayName = "Shoulders"),
		WCT_Pelvis		UMETA(DisplayName = "Pelvis"),
		WCT_Knees		UMETA(DisplayName = "Knees"),
		WCT_Feet		UMETA(DisplayName = "Feet")
	};
}

//forward declarations to handle circular dependency
//between this class and ControlInputStateBase.h
class UControlInputStateBase;
class UInputStateCharacterAnimInstance;

UCLASS()
class THIRDPERSCONTROLLER_API AInputStateMachineCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	
public:
	// Sets default values for this character's properties
	AInputStateMachineCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UInputStateCharacterAnimInstance *CharAnimBP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSprintButtonPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsJumpButtonPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanTrace; //if true, run the "FindNearbyWalls" functions. Currently unused in C++ or Blueprint

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		bool bLeftHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IKHelpers")
		bool bRightHandAgainstWall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UChildActorComponent* ShoulderPositionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UChildActorComponent* PelvisPositionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UChildActorComponent* KneePositionComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UChildActorComponent* FootPositionComponent;


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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FWallProjectionLocation FootToWallHeight;

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

	UFUNCTION(BlueprintCallable, Category = "State Machine")
		void SetCurrentState(TSubclassOf<UControlInputStateBase> newState);

	UFUNCTION(BlueprintCallable, Category = "State Machine")
		void SetCurrentAnimState(TEnumAsByte<EInputState::InputState> newState);

	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="TraceColor,TraceHitColor"))
		void FindNearbyWalls(const UChildActorComponent* Position, const float Distance, /*EDrawDebugTrace::Type is missing, dunno what to replace it with?*/
			FLinearColor TraceColor, FLinearColor TraceHitColor, UPARAM(ref) FWallProjectionLocation& WallLoc);

	UFUNCTION(BlueprintCallable)
		void FindNearbyLedges(const float DistanceFromCharacter, const float HeightAboveCharacter, FLinearColor TraceColor, FLinearColor TraceHitLinearColor);

	UFUNCTION(BlueprintCallable)
		void TraceIKHandToWall(const FName& SocketName, const float& TraceDistance);
};
