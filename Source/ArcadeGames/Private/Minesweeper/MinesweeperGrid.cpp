// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperGrid.h"
#include "Minefield.h"

AMinesweeperGrid::AMinesweeperGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	// Set defaults
	iWidth = 5;
	iHeight = 5;
	iTileSize = 100.f;
}

void AMinesweeperGrid::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateGrid();
}

void AMinesweeperGrid::ShowAllMines()
{
	for (auto m : MineFields)
	{
		m->ShowMine();
	}
}

void AMinesweeperGrid::IsMine(int x, int y)
{

}

void AMinesweeperGrid::FindAllEmptyNeighbours(AMineField* MineField)
{
	FVector2D index = ConvertWorldLocationToGridIndex(MineField->GetActorLocation());

	if (GridMap.Contains(FVector2D(index.X - 1, index.Y)) &&
		GridMap[FVector2D(index.X - 1, index.Y)]->IsMine() == false &&
		MineCountOnNeighbours(GridMap[FVector2D(index.X - 1, index.Y)]) == 0)
	{
		GridMap[FVector2D(index.X - 1, index.Y)]->OpenField();
	}


	if (GridMap.Contains(FVector2D(index.X, index.Y - 1)) &&
		GridMap[FVector2D(index.X, index.Y - 1)]->IsMine() == false &&
		MineCountOnNeighbours(GridMap[FVector2D(index.X, index.Y - 1)]) == 0)
	{
		GridMap[FVector2D(index.X, index.Y - 1)]->OpenField();
	}

	if (GridMap.Contains(FVector2D(index.X, index.Y + 1)) &&
		GridMap[FVector2D(index.X, index.Y + 1)]->IsMine() == false &&
		MineCountOnNeighbours(GridMap[FVector2D(index.X, index.Y + 1)]) == 0)
	{
		GridMap[FVector2D(index.X, index.Y + 1)]->OpenField();
	}

	if (GridMap.Contains(FVector2D(index.X + 1, index.Y)) &&
		GridMap[FVector2D(index.X + 1, index.Y)]->IsMine() == false &&
		MineCountOnNeighbours(GridMap[FVector2D(index.X + 1, index.Y)]) == 0)
	{
		GridMap[FVector2D(index.X + 1, index.Y)]->OpenField();
	}
}

int AMinesweeperGrid::MineCountOnNeighbours(AMineField* MineField)
{
	int MineCount = 0;
	FVector2D index = ConvertWorldLocationToGridIndex(MineField->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("==================="));

	if (GridMap.Contains(FVector2D(index.X - 1, index.Y - 1)) &&
		GridMap[FVector2D(index.X - 1, index.Y - 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("SOUTH WEST"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X - 1, index.Y)) &&
		GridMap[FVector2D(index.X - 1, index.Y)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("WEST"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X - 1, index.Y + 1)) &&
		GridMap[FVector2D(index.X - 1, index.Y + 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("NORTH WEST"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X, index.Y - 1)) &&
		GridMap[FVector2D(index.X, index.Y - 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("SOUTH"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X, index.Y + 1)) &&
		GridMap[FVector2D(index.X, index.Y + 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("NORTH"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X + 1, index.Y - 1)) &&
		GridMap[FVector2D(index.X + 1, index.Y - 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("SOUTH EAST"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X + 1, index.Y)) &&
		GridMap[FVector2D(index.X + 1, index.Y)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("EAST"));
		++MineCount;
	}
	
	if (GridMap.Contains(FVector2D(index.X + 1, index.Y + 1)) &&
		GridMap[FVector2D(index.X + 1, index.Y + 1)]->IsMine())
	{
		UE_LOG(LogTemp, Warning, TEXT("NORTH EAST"));
		++MineCount;
	}
	UE_LOG(LogTemp, Warning, TEXT("MineCount: %d"), MineCount);

	return MineCount;
}

void AMinesweeperGrid::GenerateGrid()
{
	GridMap.Empty();
	for (int height = 0; height < iHeight; ++height)
	{
		for (int width = 0; width < iWidth; ++width)
		{
			const float YOffset = width * iTileSize; // horizontal
			const float ZOffset = height * iTileSize; // vertical

			// Make position vector, offset from Grid location
			const FVector Loc = FVector(0.f, YOffset, ZOffset) + GetActorLocation();

			// Spawn a block
			AMineField* MineField = GetWorld()->SpawnActor<AMineField>(MineFieldClass, Loc, FRotator(0, 0, 0));
			if (MineField)
			{
				// Tell the block about its owner
				MineField->Grid = this;
				MineFields.Add(MineField);
				GridMap.Add(FVector2D(width, height), MineField);
				if (FMath::RandRange(0, 10) < 2)
				{
					MineField->SetMine(true);
				}
			}
		}
	}
}

FVector AMinesweeperGrid::ConvertGridIndexToWorldLocation(FVector2D index)
{
	FVector GridLoc = GetActorLocation();

	float X = GridLoc.X;
	float Y = GridLoc.Y + (index.X * iTileSize);
	float Z = GridLoc.Z + (index.Y * iTileSize);

	return FVector(X, Y, Z);
}

FVector2D AMinesweeperGrid::ConvertWorldLocationToGridIndex(FVector Loc)
{
	FVector GridLoc = GetActorLocation();

	int X = FMath::RoundToInt((Loc.Y - GridLoc.Y) / iTileSize);
	int Y = FMath::RoundToInt((Loc.Z - GridLoc.Z) / iTileSize);

	return FVector2D(X, Y);
}

