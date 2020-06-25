// Fill out your copyright notice in the Description page of Project Settings.


#include "PacManGrid.h"

APacManGrid::APacManGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

}

void APacManGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

void APacManGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void APacManGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APacManGrid::GenerateGrid()
{
	GridMap.Empty();
	for (int i = 0; i < iWidth; ++i)
	{
		for (int j = 0; j < iHeight; ++j)
		{
			GridMap.Add(FVector2D(i, j), nullptr);
		}
	}
}

FVector APacManGrid::ConvertGridIndexToWorldLocation(FVector2D index)
{
	FVector GridLoc = GetActorLocation();

	float X = GridLoc.X;
	float Y = GridLoc.Y + (index.X * iTileSize);
	float Z = GridLoc.Z + (index.Y * iTileSize);

	return FVector(X, Y, Z);
}

FVector2D APacManGrid::ConvertWorldLocationToGridIndex(FVector Loc)
{
	FVector GridLoc = GetActorLocation();

	int X = FMath::RoundToInt((Loc.Y - GridLoc.Y) / iTileSize);
	int Y = FMath::RoundToInt((Loc.Z - GridLoc.Z) / iTileSize);

	return FVector2D(X, Y);
}

