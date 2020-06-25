// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockGrid.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

// Sets default values
ABlockGrid::ABlockGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;

	
	Player1Tooltip = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player1Tooltip"));
	Player1Tooltip->SetRelativeLocation(FVector(-600.f, 500.f, 0.f));
	Player1Tooltip->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player1Tooltip->SetText(FText(LOCTEXT("P1", "P1")));
	Player1Tooltip->SetTextRenderColor(FColor::Black);
	Player1Tooltip->SetWorldSize(72.f);
	Player1Tooltip->SetupAttachment(DummyRoot);

	Player2Tooltip = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player2Tooltip"));
	Player2Tooltip->SetRelativeLocation(FVector(-600.f, -1100.f, 0.f));
	Player2Tooltip->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player2Tooltip->SetText(FText(LOCTEXT("P2", "P2")));
	Player2Tooltip->SetTextRenderColor(FColor::Red);
	Player2Tooltip->SetWorldSize(72.f);
	Player2Tooltip->SetupAttachment(DummyRoot);

	Player1Wins = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player1Wins"));
	Player1Wins->SetRelativeLocation(FVector(-600.f, 400.f, 0.f));
	Player1Wins->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player1Wins->SetText(FText(LOCTEXT("P1_Score", "0")));
	Player1Wins->SetTextRenderColor(FColor::Black);
	Player1Wins->SetWorldSize(144.f);
	Player1Wins->SetupAttachment(DummyRoot);

	Player2Wins = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player2Wins"));
	Player2Wins->SetRelativeLocation(FVector(-600.f, -1000.f, 0.f));
	Player2Wins->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	Player2Wins->SetText(FText(LOCTEXT("P2_Score", "0")));
	Player2Wins->SetTextRenderColor(FColor::Red);
	Player2Wins->SetWorldSize(144.f);
	Player2Wins->SetupAttachment(DummyRoot);

	GameState = CreateDefaultSubobject<UTextRenderComponent>(TEXT("GameState"));
	GameState->SetRelativeLocation(FVector(230.f, -300.f, 0.f));
	GameState->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	GameState->SetText(FText(LOCTEXT("Game_State", "Game State")));
	GameState->SetTextRenderColor(FColor::Black);
	GameState->SetWorldSize(48.f);
	GameState->SetupAttachment(DummyRoot);

}

void ABlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Spawn each block
	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; ++BlockIndex)
	{
		const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockLocation, FRotator(0, 0, 0));
		BlockArray.Add(NewBlock);
		EmptyBlock = BlockArray.Num();

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
		}

		OnRefreshMoveText();
	}
}

bool ABlockGrid::IsOwner(EBlockState PlayerSide, int32 BlockIndex)
{
	return BlockArray[BlockIndex]->GetPlayer() == PlayerSide;
}

bool ABlockGrid::CheckWin(EBlockState PlayerSide)
{
	if (IsOwner(PlayerSide, 0) && IsOwner(PlayerSide, 1) && IsOwner(PlayerSide, 2))
		return true;

	if (IsOwner(PlayerSide, 3) && IsOwner(PlayerSide, 4) && IsOwner(PlayerSide, 5))
		return true;

	if (IsOwner(PlayerSide, 6) && IsOwner(PlayerSide, 7) && IsOwner(PlayerSide, 8))
		return true;

	if (IsOwner(PlayerSide, 0) && IsOwner(PlayerSide, 3) && IsOwner(PlayerSide, 6))
		return true;

	if (IsOwner(PlayerSide, 1) && IsOwner(PlayerSide, 4) && IsOwner(PlayerSide, 7))
		return true;

	if (IsOwner(PlayerSide, 2) && IsOwner(PlayerSide, 5) && IsOwner(PlayerSide, 8))
		return true;

	if (IsOwner(PlayerSide, 0) && IsOwner(PlayerSide, 4) && IsOwner(PlayerSide, 8))
		return true;

	if (IsOwner(PlayerSide, 2) && IsOwner(PlayerSide, 4) && IsOwner(PlayerSide, 6))
		return true;

	return false;
}

void ABlockGrid::OnMoveEnds(EBlockState PlayerSide)
{
	--EmptyBlock;
	if (EmptyBlock <= 0)
	{
		GameState->SetText(FText(LOCTEXT("Game_State", "No one wins!")));
		for (auto Block : BlockArray)
		{
			Block->Restart();
		}
	}
	else if (CheckWin(PlayerSide))
	{
		GameState->SetText((FText::Format(LOCTEXT("Game_State", "Player {0} Wins!"), FText::AsNumber((uint8)PlayerSide))));
		if (PlayerSide == EBlockState::T_P1)
		{
			++P1Score;
			Player1Wins->SetText((FText::Format(LOCTEXT("P1_Score", "{0}"), FText::AsNumber(P1Score))));
		}
		else
		{
			++P2Score;
			Player2Wins->SetText((FText::Format(LOCTEXT("P2_Score", "{0}"), FText::AsNumber(P2Score))));
		}

		for (auto Block : BlockArray)
		{
			Block->Restart();
		}

		EmptyBlock = BlockArray.Num();

		if (P1Score > 3 || P2Score > 3)
			GameOver();

	}
	else
	{
		OnRefreshMoveText();
	}
}

void ABlockGrid::OnRefreshMoveText()
{
	if (bIsX)
		GameState->SetText(FText(LOCTEXT("Game_State", "Player 2 moves")));
	else
		GameState->SetText(FText(LOCTEXT("Game_State", "Player 1 moves")));
}

void ABlockGrid::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("HI"));
}

#undef LOCTEXT_NAMESPACE
