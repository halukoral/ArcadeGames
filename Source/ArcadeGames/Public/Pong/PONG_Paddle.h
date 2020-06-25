// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PONG_Ball.h"
#include "GameFramework/Pawn.h"
#include "PONG_Paddle.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMovementMethod : uint8
{
	MV_Location 	UMETA(DisplayName = "Move With SetActorLocation"),
	MV_Force 		UMETA(DisplayName = "Move With AddForce")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPlayer : uint8
{
	P_P1 	UMETA(DisplayName = "P1"),
	P_P2	UMETA(DisplayName = "P2"),
	P_AI	UMETA(DisplayName = "AI")
};

UCLASS()
class ARCADEGAMES_API APONG_Paddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APONG_Paddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(float AxisValue);
	void MoveWithSetActorLocation(float DeltaTime);
	void MoveWithAddForce(float DeltaTime);
	void AIMovement();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Camera)
	AActor* MainCamera = nullptr;

private:
	// Input variables
	FVector MovementInput;

	APONG_Ball* Ball;

	UPROPERTY()
	bool bAllowEditPlayer = true;
	UPROPERTY()
	bool bAllowEditAI = true;

	UPROPERTY(EditAnywhere, Category = Input)
	EPlayer PlayerEnum;

	UPROPERTY(EditAnywhere, Category = Input, meta = (EditCondition = bAllowEditPlayer))
	EMovementMethod MovementEnum;

	UPROPERTY(EditAnywhere, Category = Input, meta = (EditCondition = bAllowEditPlayer))
	float MovementSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = Input, meta = (EditCondition = bAllowEditPlayer))
	float ForceMultiplier = 500000.0f;

	UPROPERTY(EditAnywhere, Category = Input, meta = (EditCondition = bAllowEditAI))
	float AIPaddleSpeed = 50000.0f;

	UPROPERTY(EditAnywhere, Category = Input, meta = (EditCondition = bAllowEditAI))
	float AIPaddleMovementError = 1.0f;
};
