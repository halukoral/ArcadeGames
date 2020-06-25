// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisPawn.h"
#include "TetrisGrid.h"
#include "Kismet/GameplayStatics.h"

ATetrisPawn::ATetrisPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATetrisPawn::BeginPlay()
{
	Super::BeginPlay();
	pGrid = Cast<ATetrisGrid>(UGameplayStatics::GetActorOfClass(this, ATetrisGrid::StaticClass()));
}

void ATetrisPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find movement direction
	const float ForwardValue = GetInputAxisValue("MoveY");
	const float RightValue = GetInputAxisValue("MoveX");

	bGoDown = bGoRight = bGoLeft = false;

	SensitivityXTimer += DeltaTime;
	SensitivityYTimer += DeltaTime;

	if (SensitivityYTimer >= SensitivityY && ForwardValue < -0.7)
	{
		bGoDown = true;
		AxisValue = ForwardValue;
		SensitivityYTimer = 0.f;
	}

	if (SensitivityXTimer >= SensitivityX && RightValue > 0.7)
	{
		bGoRight = true;
		AxisValue = RightValue;
		SensitivityXTimer = 0.f;
	}
	else if (SensitivityXTimer >= SensitivityX && RightValue < -0.7)
	{
		bGoLeft = true;
		AxisValue = RightValue;
		SensitivityXTimer = 0.f;
	}

	if (bGoDown)
		Move(AxisValue, DeltaTime, true);

	else if (bGoRight || bGoLeft)
		Move(AxisValue, DeltaTime, false);
}

// Called to bind functionality to input
void ATetrisPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Tetris_HardDrop", IE_Pressed, this, &ATetrisPawn::InputHardDrop);
	PlayerInputComponent->BindAction("Tetris_Rotate", IE_Pressed, this, &ATetrisPawn::InputRotate);
	
	PlayerInputComponent->BindAxis("MoveX");
	PlayerInputComponent->BindAxis("MoveY");
}

void ATetrisPawn::Move(float AxisValue, float DeltaTime, bool AxisForward)
{
	if (pGrid && pGrid->GetActiveBlock())
	{
		if (AxisForward)
		{
			pGrid->GetActiveBlock()->MoveDown();
		}
		else if(bGoRight)
		{
			pGrid->GetActiveBlock()->MoveRight();
		}
		else if (bGoLeft)
		{
			pGrid->GetActiveBlock()->MoveLeft();
		}
	}

}
void ATetrisPawn::InputHardDrop()
{
	if (pGrid && pGrid->GetActiveBlock())
	{
		pGrid->GetActiveBlock()->MoveHardDrop();
	}
}

void ATetrisPawn::InputRotate()
{
	if (pGrid && pGrid->GetActiveBlock())
	{
		pGrid->GetActiveBlock()->Rotate();
	}
}