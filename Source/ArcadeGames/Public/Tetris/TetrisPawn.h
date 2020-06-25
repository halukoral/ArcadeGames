// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TetrisPawn.generated.h"

class ATetrisGrid;

UCLASS()
class ARCADEGAMES_API ATetrisPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATetrisPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Move(float AxisValue, float DeltaTime, bool AxisForward);
	void InputHardDrop();
	void InputRotate();

private:

	bool  bGoDown;
	bool  bGoLeft;
	bool  bGoRight;
	bool  bLeftPressed;
	bool  bRightPressed;

	float AxisValue;
	float SensitivityX = 0.15f;
	float SensitivityY = 0.15f;

	float SensitivityXTimer = 0.f;
	float SensitivityYTimer = 0.f;

	ATetrisGrid* pGrid = nullptr;
};
