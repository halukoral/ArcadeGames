// Fill out your copyright notice in the Description page of Project Settings.


#include "TetrisGrid.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"

ATetrisGrid::ATetrisGrid()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	TetrisShadow = CreateDefaultSubobject<UChildActorComponent>(TEXT("TetrisShadow"));
	RootComponent = SceneComponent;
	TetrisShadow->SetupAttachment(SceneComponent);

	for (int i = 0; i < iBlockCount; ++i)
	{
		UChildActorComponent* Child = CreateDefaultSubobject<UChildActorComponent>(*("TetrisBlock_" + FString::FromInt(i)));
		if (Child)
		{
			Child->SetupAttachment(SceneComponent);
			TetrisBlocks.Add(Child);
		}
	}
}

void ATetrisGrid::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto e : TetrisBlocks)
	{
		if (e)
			e->SetChildActorClass(nullptr);
	}
	StartGame(true);
}

void ATetrisGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	StartGame(false);
}

void ATetrisGrid::StartGame(bool bStarted)
{
	iCurrentLevel				= 1;
	iCurrentCombo				= 0;
	iCurrentPoints				= 0;
	iCurrentCompletedRowCount	= 0;
	fGameSpeed					= 1.f;
	pActiveBlock				= nullptr;

	LastScore					= ETetrisScore::None;
	
	GenerateGrid();

	for (int i = 0; i < TetrisBlocks.Num(); ++i)
	{
		TetrisBlocks[i]->SetRelativeLocation(FVector(0.f, (iWidth + 2) * iTileSize, (4 * i) * iTileSize));
	}
	UpdateTetrisBlocks();

	if (bStarted)
	{
		SpawnNewBlock();
	}
}

void ATetrisGrid::TetrisBlockReachedGround()
{
	if (pActiveBlock)
	{
		pActiveBlock->SetActive(false);
		pActiveBlock = nullptr;
	}
	RemoveFullRows();
	SpawnNewBlock();
}

void ATetrisGrid::RemoveFullRows()
{
	int CompletedRow = 0;
	for (int i = 0; i < iHeight; ++i)
	{
		if (IsRowFull(i))
		{
			DeleteRow(i);
			DecreaseRows(i);
			--i;
			++iCurrentCompletedRowCount;
			++CompletedRow;

			if (iCurrentCompletedRowCount % 10 == 0)
			{
				LevelUp();
			}
		}
	}
	if (CompletedRow > 0)
	{
		++iCurrentCombo;

		switch (CompletedRow)
		{
		case 1:
			IncreasePoints(ETetrisScore::SingleRow);
			break;
		case 2:
			IncreasePoints(ETetrisScore::DoubleRow);
			break;
		case 3:
			IncreasePoints(ETetrisScore::TripleRow);
			break;
		case 4:
			if (LastScore == ETetrisScore::Tetris || LastScore == ETetrisScore::BackToBackTetris)
				IncreasePoints(ETetrisScore::BackToBackTetris);
			else
				IncreasePoints(ETetrisScore::Tetris);
			break;
		default:
			break;
		}

		bool bIsCleared = true;
		for (int i = 0; i < iWidth; ++i)
		{
			if (GridMap[FVector2D(i, 0)])
			{
				bIsCleared = false;
				break;
			}
		}

		if (bIsCleared)
			IncreasePoints(ETetrisScore::FullClear);
	}
	else
	{
		iCurrentCombo = 0;
		LastScore = ETetrisScore::None;
	}
}

void ATetrisGrid::DecreaseRows(int index)
{
	for (int i = index + 1; i < iHeight; ++i)
	{
		for (int j = 0; j < iWidth; ++j)
		{
			if (GridMap[FVector2D(j, i)])
			{
				GridMap[FVector2D(j, i)]->AddWorldOffset(FVector(0.f,0.f,-1.f * iTileSize));
			}
			GridMap.Add(FVector2D(j, i - 1), GridMap[FVector2D(j, i)]);
			GridMap.Add(FVector2D(j,i), nullptr);
		}
	}
}

void ATetrisGrid::DeleteRow(int index)
{
	for (int i = 0; i < iWidth; ++i)
	{
		GridMap[FVector2D(i, index)]->SetHiddenInGame(true);

		auto* TetrisBlock = Cast<ATetrisBlock>(GridMap[FVector2D(i,index)]->GetOwner());
		if (TetrisBlock)
		{
			if (TetrisBlock->Block01->bHiddenInGame && 
				TetrisBlock->Block02->bHiddenInGame && 
				TetrisBlock->Block03->bHiddenInGame && 
				TetrisBlock->Block04->bHiddenInGame)
			{
				PoolTetrisBlock(TetrisBlock);
			}
		}
		GridMap.Add(FVector2D(i, index), nullptr);
	}
}

bool ATetrisGrid::IsRowFull(int index)
{
	for (int i = 0; i < iWidth; ++i)
	{
		if(GridMap[FVector2D(i, index)] == nullptr)
			return false;
	}
	return true;
}

bool ATetrisGrid::IsValidIndex(TArray<FVector2D> Target, TArray<FVector2D> Current)
{
	for (auto i : Target)
	{
		if (!Current.Contains(i))
		{
			if (!GridMap.Contains(i))
			{
				return false;
			}
			else if (GridMap[i])
			{
				return false;
			}
		}
	}
	return true;
}

void ATetrisGrid::LevelUp()
{
	if (iCurrentLevel < iMaxLevel)
	{
		++iCurrentLevel;
		fGameSpeed -= fGameSpeed / 3.f;
	}
}

void ATetrisGrid::GameOver()
{

}

void ATetrisGrid::PoolTetrisBlock(ATetrisBlock* Block)
{
	if (Block)
	{
		Block->SetActive(false);
		Block->Block01->SetHiddenInGame(true);
		Block->Block02->SetHiddenInGame(true);
		Block->Block03->SetHiddenInGame(true);
		Block->Block04->SetHiddenInGame(true);
		PooledBlocks.AddUnique(Block);
	}
}

void ATetrisGrid::IncreasePoints(ETetrisScore e)
{
	switch (e)
	{
	case ETetrisScore::SingleRow:
		iCurrentPoints += 100 * iCurrentLevel + 20 * iCurrentCombo * iCurrentLevel;
		break;

	case ETetrisScore::DoubleRow:
		iCurrentPoints += 300 * iCurrentLevel + 50 * iCurrentCombo * iCurrentLevel;
		break;

	case ETetrisScore::TripleRow:
		iCurrentPoints += 500 * iCurrentLevel + 50 * iCurrentCombo * iCurrentLevel;
		break;

	case ETetrisScore::Tetris:
		iCurrentPoints += 800 * iCurrentLevel + 50 * iCurrentCombo * iCurrentLevel;
		break;

	case ETetrisScore::BackToBackTetris:
		iCurrentPoints += 1200 * iCurrentLevel + 50 * iCurrentCombo * iCurrentLevel;
		break;

	case ETetrisScore::SoftDrop:
		iCurrentPoints += 1;
		break;

	case ETetrisScore::HardDrop:
		iCurrentPoints += 2;
		break;

	case ETetrisScore::FullClear:
		iCurrentPoints += 5000 * iCurrentLevel;
		break;

	default:
		break;
	}
	if(e != ETetrisScore::SoftDrop && e != ETetrisScore::HardDrop)
		LastScore = e;
}

void ATetrisGrid::SpawnNewBlock()
{
	FVector vSpawnLoc = ConvertGridIndexToWorldLocation(FVector2D(
		FMath::RoundToInt(iWidth/2 - 1), FMath::RoundToInt(iHeight - 3)
	));

	ETetrisBlock Block = eTetrisBlocks[0];
	pActiveBlock = GetNewTetrisBlock(vSpawnLoc, Block);
	if (pActiveBlock)
	{
		TArray<FVector2D> ActiveBlockGridIndexes = pActiveBlock->GetGridIndex();
		if (IsValidIndex(ActiveBlockGridIndexes, TArray<FVector2D>()))
		{
			GridMap.Add(ActiveBlockGridIndexes[0], pActiveBlock->Block01);
			GridMap.Add(ActiveBlockGridIndexes[1], pActiveBlock->Block02);
			GridMap.Add(ActiveBlockGridIndexes[2], pActiveBlock->Block03);
			GridMap.Add(ActiveBlockGridIndexes[3], pActiveBlock->Block04);
			pActiveBlock->SetActive(true);
		}
		else
		{
			pActiveBlock->SetActive(false);
			GameOver();
		}
	}
	eTetrisBlocks.RemoveAt(0);
	UpdateTetrisBlocks();
	UpdateTetrisShadows();
}

ATetrisBlock* ATetrisGrid::GetNewTetrisBlock(FVector v, ETetrisBlock b)
{
	ATetrisBlock* TetrisBlock = nullptr;

	if (TetrisBlockClass)
	{
		if (PooledBlocks.Num() > 0)
		{
			TetrisBlock = PooledBlocks[0];
			PooledBlocks.RemoveAt(0);
			if (TetrisBlock)
			{
				TetrisBlock->SetActorLocationAndRotation(v, FRotator::ZeroRotator);
				TetrisBlock->Block01->SetHiddenInGame(false);
				TetrisBlock->Block02->SetHiddenInGame(false);
				TetrisBlock->Block03->SetHiddenInGame(false);
				TetrisBlock->Block04->SetHiddenInGame(false);
			}
		}
		else
		{
			TetrisBlock = GetWorld()->SpawnActor<ATetrisBlock>(TetrisBlockClass, v, FRotator::ZeroRotator);
			if (TetrisBlock)
				SpawnedBlocks.Add(TetrisBlock);
		}

		if (TetrisBlock)
		{
			TetrisBlock->SetBlocksMesh(b);
			TetrisBlock->SetBlockTransform(iTileSize);
			TetrisBlock->SetBlockMesh();
			TetrisBlock->SetTetrisGrid(this);
		}
	}
	return TetrisBlock;
}

void ATetrisGrid::GenerateGrid()
{
	for (auto e : SpawnedBlocks)
	{
		PoolTetrisBlock(e);
	}

	GridMap.Empty();
	for (int i = 0; i < iWidth; ++i)
	{
		for (int j = 0; j < iHeight; ++j)
		{
			GridMap.Add(FVector2D(i, j), nullptr);
		}
	}
}

FVector ATetrisGrid::ConvertGridIndexToWorldLocation(FVector2D index)
{
	FVector GridLoc = GetActorLocation();
	
	float X = GridLoc.X;
	float Y = GridLoc.Y + (index.X * iTileSize);
	float Z = GridLoc.Z + (index.Y * iTileSize);

	return FVector(X, Y, Z);
}

FVector2D ATetrisGrid::ConvertWorldLocationToGridIndex(FVector Loc)
{
	FVector GridLoc = GetActorLocation();

	int X = FMath::RoundToInt((Loc.Y - GridLoc.Y) / iTileSize);
	int Y = FMath::RoundToInt((Loc.Z - GridLoc.Z) / iTileSize);
	
	return FVector2D(X, Y);
}

void ATetrisGrid::UpdateTetrisBlocks()
{
	while (eTetrisBlocks.Num() < 8)
	{
		TArray<ETetrisBlock> EveryType = { ETetrisBlock::I, ETetrisBlock::J, ETetrisBlock::L, ETetrisBlock::O,
											ETetrisBlock::S, ETetrisBlock::T, ETetrisBlock::Z };

		while (EveryType.Num() > 0)
		{
			int i = FMath::RandRange(0, EveryType.Num() - 1);
			eTetrisBlocks.Add(EveryType[i]);
			EveryType.RemoveAt(i);
		}
	}

	if (TetrisBlockClass)
	{
		for (int i = 0; i < TetrisBlocks.Num(); ++i)
		{
			if (TetrisBlocks[i])
			{
				ATetrisBlock* Block = Cast<ATetrisBlock>(TetrisBlocks[i]->GetChildActor());
				if (Block == nullptr)
				{
					TetrisBlocks[i]->SetChildActorClass(TetrisBlockClass);
					Block = Cast<ATetrisBlock>(TetrisBlocks[i]->GetChildActor());
				}
				if (Block)
				{
					Block->SetTetrisGrid(this);
					Block->SetBlocksMesh(eTetrisBlocks[i]);
					Block->SetBlockTransform(iTileSize);
					Block->SetBlockMesh();
				}
			}
		}
	}
}

void ATetrisGrid::UpdateTetrisShadows()
{
	if (TetrisBlockClass)
	{
		if (TetrisShadow)
		{
			ATetrisBlock* Block = Cast<ATetrisBlock>(TetrisShadow->GetChildActor());
			if (Block == nullptr)
			{
				TetrisShadow->SetChildActorClass(TetrisBlockClass);
				Block = Cast<ATetrisBlock>(TetrisShadow->GetChildActor());
			}
			if (Block && pActiveBlock)
			{
				Block->SetTetrisGrid(this);
				Block->SetShadow(true);
				Block->SetBlocksMesh(pActiveBlock->GetBlockMesh());
				Block->SetBlockTransform(iTileSize);
				Block->SetActorTransform(pActiveBlock->GetActorTransform());
				Block->SetBlockMesh();

				TArray<FVector2D> InitGridIndexes = Block->GetGridIndex(FVector2D::ZeroVector);
				while (IsValidIndex(Block->GetGridIndex(FVector2D(0.f, -1.f)), InitGridIndexes))
				{
					Block->AddActorWorldOffset(FVector(0.f, 0.f, -1.f * iTileSize));
				}
			}
		}
	}
}

