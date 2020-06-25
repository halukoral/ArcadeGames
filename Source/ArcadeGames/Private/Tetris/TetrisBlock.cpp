// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisBlock.h"
#include "TetrisGrid.h"
#include "Materials/MaterialInstanceDynamic.h"


ATetrisBlock::ATetrisBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	RootComponent = SceneComponent;

	Block01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh01"));
	Block02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh02"));
	Block03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh03"));
	Block04 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh04"));

	Block01->SetupAttachment(SceneComponent);
	Block02->SetupAttachment(SceneComponent);
	Block03->SetupAttachment(SceneComponent);
	Block04->SetupAttachment(SceneComponent);

	SetBlockMesh();
}

void ATetrisBlock::BeginPlay()
{
	Super::BeginPlay();
}

void ATetrisBlock::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SetBlockTransform(100.f);
	SetBlockMesh();
}

void ATetrisBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bActive)
	{
		MoveDown(DeltaTime);
	}
}

void ATetrisBlock::SetBlockTransform(float TileSize)
{
	Block01->SetRelativeScale3D(FVector(TileSize / 100.f));
	Block02->SetRelativeScale3D(FVector(TileSize / 100.f));
	Block03->SetRelativeScale3D(FVector(TileSize / 100.f));
	Block04->SetRelativeScale3D(FVector(TileSize / 100.f));

	Block01->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	Block02->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	Block03->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	Block04->SetWorldRotation(FRotator(90.f, 0.f, 0.f));

	switch (eShape)
	{
	case ETetrisBlock::I:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, 0.f));
		Block02->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block03->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, TileSize * 2, 0.f));
		break;

	case ETetrisBlock::J:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, TileSize));
		Block02->SetRelativeLocation(FVector(0.f, -TileSize, 0.f));
		Block03->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		break;

	case ETetrisBlock::L:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, 0.f));
		Block02->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block03->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, TileSize, TileSize));
		break;

	case ETetrisBlock::O:
		Block01->SetRelativeLocation(FVector(0.f, 0.f, TileSize));
		Block02->SetRelativeLocation(FVector(0.f, TileSize, TileSize));
		Block03->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		break;

	case ETetrisBlock::S:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, 0.f));
		Block02->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block03->SetRelativeLocation(FVector(0.f, 0.f, TileSize));
		Block04->SetRelativeLocation(FVector(0.f, TileSize, TileSize));
		break;

	case ETetrisBlock::T:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, 0.f));
		Block02->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block03->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, 0.f, TileSize));
		break;

	case ETetrisBlock::Z:
		Block01->SetRelativeLocation(FVector(0.f, -TileSize, TileSize));
		Block02->SetRelativeLocation(FVector(0.f, 0.f, TileSize));
		Block03->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Block04->SetRelativeLocation(FVector(0.f, TileSize, 0.f));
		break;
	}
}

void ATetrisBlock::SetBlockMesh()
{
	Block01->SetStaticMesh(BlockMesh);
	Block02->SetStaticMesh(BlockMesh);
	Block03->SetStaticMesh(BlockMesh);
	Block04->SetStaticMesh(BlockMesh);

	if (DynamicMaterial == nullptr && TetrisBlockMaterial != nullptr)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(TetrisBlockMaterial, this, "TetrisBlockMat");
		Block01->SetMaterial(0, DynamicMaterial);
		Block02->SetMaterial(0, DynamicMaterial);
		Block03->SetMaterial(0, DynamicMaterial);
		Block04->SetMaterial(0, DynamicMaterial);
	}

	FLinearColor Color;
	if (!bShadow)
	{
		switch (eShape)
		{
		case ETetrisBlock::I:
			Color = Color_I;
			break;

		case ETetrisBlock::J:
			Color = Color_J;
			break;

		case ETetrisBlock::L:
			Color = Color_L;
			break;

		case ETetrisBlock::O:
			Color = Color_O;
			break;

		case ETetrisBlock::S:
			Color = Color_S;
			break;

		case ETetrisBlock::T:
			Color = Color_T;
			break;

		case ETetrisBlock::Z:
			Color = Color_Z;
			break;
		}
	}
	else
	{
		Color = Color_Shadow;
	}
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", Color);
	}
}

void ATetrisBlock::MoveHardDrop()
{
	while (Move(FVector2D(0.f, -1.f)))
	{
		fTimeSinceLastMove = 0.f;
	}
	if (TetrisGrid)
	{
		TetrisGrid->TetrisBlockReachedGround();	
		TetrisGrid->IncreasePoints(ETetrisScore::HardDrop);
	}
}

void ATetrisBlock::Rotate()
{
	if (eShape == ETetrisBlock::O)
		return;

	FRotator DesiredRotation = FRotator(0.f, 0.f, 90.f);

	if (eShape == ETetrisBlock::I)
	{
		if(GetActorRotation().Equals(FRotator::ZeroRotator, 5.f))
			DesiredRotation = FRotator(0.f, 0.f, -90.f);
	}
	if (Rotate(DesiredRotation))
	{
		if (TetrisGrid)
		{
			TetrisGrid->UpdateTetrisShadows();
		}
	}
}

bool ATetrisBlock::Rotate(FRotator Rotation)
{
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();

	TArray<FVector2D> Current = GetGridIndex();

	AddActorWorldRotation(Rotation);

	TArray<FVector2D> LocToTry = { FVector2D::ZeroVector };
	LocToTry.Append({ FVector2D(1.f,0.f),FVector2D(-1.f,0.f) });
	
	if(eShape == ETetrisBlock::I)
		LocToTry.Add({ FVector2D(-2.f,0.f) });

	if (TetrisGrid)
	{
		for (auto Location : LocToTry)
		{
			SetActorLocation(Loc);
			AddActorWorldOffset(FVector(0.f, Location.X * TetrisGrid->GetTileSize(), Location.Y * TetrisGrid->GetTileSize()));
			TArray<FVector2D> Target = GetGridIndex(FVector2D::ZeroVector);
			if (TetrisGrid->IsValidIndex(Target, Current))
			{
				// Remove Block From Grid
				TetrisGrid->GridMap.Add(Current[0], nullptr);
				TetrisGrid->GridMap.Add(Current[1], nullptr);
				TetrisGrid->GridMap.Add(Current[2], nullptr);
				TetrisGrid->GridMap.Add(Current[3], nullptr);

				// Add Block To Grid
				TetrisGrid->GridMap.Add(Target[0], Block01);
				TetrisGrid->GridMap.Add(Target[1], Block02);
				TetrisGrid->GridMap.Add(Target[2], Block03);
				TetrisGrid->GridMap.Add(Target[3], Block04);

				Block01->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
				Block02->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
				Block03->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
				Block04->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
				return true;
			}
		}
	}
	SetActorRotation(Rot);
	SetActorLocation(Loc);
	return false;
}

bool ATetrisBlock::Move(FVector2D Direction)
{
	TArray<FVector2D> Current = GetGridIndex();
	TArray<FVector2D> Target = GetGridIndex(FVector2D(Direction.X, Direction.Y));

	if (TetrisGrid && TetrisGrid->IsValidIndex(Target, Current))
	{
		// Move Block
		AddActorWorldOffset(FVector(0.f, Direction.X * TetrisGrid->GetTileSize(), Direction.Y * TetrisGrid->GetTileSize()));

		// Remove Block From Grid
		TetrisGrid->GridMap.Add(Current[0], nullptr);
		TetrisGrid->GridMap.Add(Current[1], nullptr);
		TetrisGrid->GridMap.Add(Current[2], nullptr);
		TetrisGrid->GridMap.Add(Current[3], nullptr);

		// Add Block To Grid
		TetrisGrid->GridMap.Add(Target[0], Block01);
		TetrisGrid->GridMap.Add(Target[1], Block02);
		TetrisGrid->GridMap.Add(Target[2], Block03);
		TetrisGrid->GridMap.Add(Target[3], Block04);

		TetrisGrid->UpdateTetrisShadows();

		return true;
	}
	return false;
}

void ATetrisBlock::MoveDown(float DeltaTime)
{
	fTimeSinceLastMove += DeltaTime;

	if (TetrisGrid && fTimeSinceLastMove >= TetrisGrid->GameSpeed())
	{
		if (!Move(FVector2D(0.f, -1.f)))
		{
			if (fTimeSinceLastMove >= TetrisGrid->GameSpeed() + TetrisGrid->AdditionalTime())
			{
				TetrisGrid->TetrisBlockReachedGround();
				TetrisGrid->IncreasePoints(ETetrisScore::SoftDrop);
				fTimeSinceLastMove = 0.f;
			}
		}
		else
		{
			fTimeSinceLastMove = 0.f;
		}
	}
}

// Move Down By User Input
void ATetrisBlock::MoveDown()
{
	if (!Move(FVector2D(0.f, -1.f)) && TetrisGrid)
	{
		TetrisGrid->TetrisBlockReachedGround();
		TetrisGrid->IncreasePoints(ETetrisScore::SoftDrop);
		fTimeSinceLastMove = 0.f;
	}
}

TArray<FVector2D> ATetrisBlock::GetGridIndex(FVector2D vOffset)
{
	if (TetrisGrid)
	{
		FVector2D Block01_Index = TetrisGrid->ConvertWorldLocationToGridIndex(
			Block01->GetComponentToWorld().GetLocation()) + vOffset;

		FVector2D Block02_Index = TetrisGrid->ConvertWorldLocationToGridIndex(
			Block02->GetComponentToWorld().GetLocation()) + vOffset;

		FVector2D Block03_Index = TetrisGrid->ConvertWorldLocationToGridIndex(
			Block03->GetComponentToWorld().GetLocation()) + vOffset;

		FVector2D Block04_Index = TetrisGrid->ConvertWorldLocationToGridIndex(
			Block04->GetComponentToWorld().GetLocation()) + vOffset;

		return { Block01_Index, Block02_Index, Block03_Index, Block04_Index };
	}

	return TArray<FVector2D>();
}

