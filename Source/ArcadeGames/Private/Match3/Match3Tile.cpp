// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3Tile.h"
#include "Match3Grid.h"
#include "Match3GameModeBase.h"
#include "Kismet/GameplayStatics.h"

AMatch3Tile::AMatch3Tile()
{
	PrimaryActorTick.bCanEverTick = false;
	if (RootComponent)
	{
		RootComponent->SetMobility(EComponentMobility::Movable);
	}
}

void AMatch3Tile::BeginPlay()
{
	Super::BeginPlay();
	
	Grid = Cast<AMatch3Grid>(GetOwner());

	// Set our class up to handle touch events.
	OnInputTouchBegin.AddUniqueDynamic(this, &AMatch3Tile::TilePress);
	OnInputTouchEnter.AddUniqueDynamic(this, &AMatch3Tile::TileEnter);
}

void AMatch3Tile::TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	// We clicked or touched the tile.
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		Grid->OnTileWasSelected(this);
	}
}

void AMatch3Tile::TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	// We have moved into the tile's space while we had a different tile selected. This is the same as pressing the tile directly.
	// Note that we need to make sure it's a different actual tile (i.e. not NULL) because deselecting a tile by touching it twice will then trigger the TileEnter event and re-select it.
	if (!UGameplayStatics::IsGamePaused(this) && Grid)
	{
		AMatch3Tile* CurrentlySelectedTile = Grid->GetCurrentlySelectedTile();
		if (CurrentlySelectedTile && (CurrentlySelectedTile != this))
		{
			TilePress(FingerIndex, TouchedActor);
		}
	}
}

void AMatch3Tile::TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked)
{
	TilePress(ETouchIndex::Touch1, ClickedActor);
}

void AMatch3Tile::TileEnter_Mouse(AActor* MousedOverActor)
{
	// This is meant to simulate finger-swiping, so ignore if the mouse isn't clicked.
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			TileEnter(ETouchIndex::Touch1, MousedOverActor);
		}
	}
}

void AMatch3Tile::OnMatched_Implementation(EMoveType MoveType)
{
	Grid->OnTileFinishedMatching(this);
}

void AMatch3Tile::OnSwapMove_Implementation(AMatch3Tile* OtherTile, bool bMoveWillSucceed)
{
	Grid->OnSwapDisplayFinished(this);
}



void AMatch3Tile::StartFalling(bool bUseCurrentWorldLocation)
{
	float FallDistance = 0;

	FallingStartTime = GetWorld()->GetTimeSeconds();
	FallingStartLocation = GetActorLocation();
	// Tiles fall at a fixed rate of 120 FPS.
	GetWorldTimerManager().SetTimer(TickFallingHandle, this, &AMatch3Tile::TickFalling, 0.001f, true);
	check(Grid);

	if (!bUseCurrentWorldLocation)
	{
		// Fall from where we are on the grid to where we are supposed to be on the grid.
		int32 YOffset = 0;
		int32 HeightAboveBottom = 1;
		while (true)
		{
			++YOffset;
			if (Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress))
			{
				if (AMatch3Tile* TileBelow = Grid->GetTileFromGridAddress(LandingGridAddress))
				{
					// We're not off the grid, so check to see what is in this space and react to it.
					if (TileBelow->TileState == ETileState::eFalling)
					{
						// This space contains a falling tile, so continue to fall through it, but note that the tile will land underneath us, so we need to leave a gap for it.
						++HeightAboveBottom;
						continue;
					}
					else if (TileBelow->TileState == ETileState::ePendingDelete)
					{
						// This space contains a tile that is about to be deleted. We can fall through this space freely.
						continue;
					}
				}
				else
				{
					// The space below is empty, but is on the grid. We can fall through this space freely.
					continue;
				}
			}
			// This space is off the grid or contains a tile that is staying. Go back one space and stop.
			YOffset -= HeightAboveBottom;
			Grid->GetGridAddressWithOffset(GetGridAddress(), 0, -YOffset, LandingGridAddress);
			break;
		}
		FallDistance = Grid->TileSize.Y * YOffset;
		FallingEndLocation = FallingStartLocation;
		FallingEndLocation.Z -= FallDistance;
	}
	else
	{
		// Fall from where we are physically to where we are supposed to be on the grid.
		LandingGridAddress = GetGridAddress();
		FallingEndLocation = Grid->GetLocationFromGridAddress(LandingGridAddress);
		FallDistance = FallingStartLocation.Z - FallingEndLocation.Z;
	}
	AMatch3GameModeBase* CurrentGameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(this));
	TotalFallingTime = 0.0f;
	if (CurrentGameMode && (CurrentGameMode->TileMoveSpeed > 0.0f))
	{
		TotalFallingTime = FallDistance / CurrentGameMode->TileMoveSpeed;
	}
	if (TotalFallingTime <= 0.0f)
	{
		TotalFallingTime = 0.75f;
	}
	StartFallingEffect();
}

void AMatch3Tile::TickFalling()
{
	AMatch3GameModeBase* CurrentGameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (CurrentGameMode)
	{
		check(Grid);
		check(TotalFallingTime > 0.0f);
		float FallCompleteFraction = (GetWorld()->GetTimeSeconds() - FallingStartTime) / TotalFallingTime;

		// Stop falling if we're at the final location. Otherwise, continue to move.
		if (FallCompleteFraction >= 1.0f)
		{
			FinishFalling();
		}
		else
		{
			FVector NewLocation = FMath::Lerp(FallingStartLocation, FallingEndLocation, FallCompleteFraction);
			SetActorLocation(NewLocation);
		}
	}
	else
	{
		// Error. Stop ticking this function. Move the tile to the final location.
		FinishFalling();
	}
}

void AMatch3Tile::FinishFalling()
{
	GetWorldTimerManager().ClearTimer(TickFallingHandle);
	SetActorLocation(FallingEndLocation);
	Grid->OnTileFinishedFalling(this, LandingGridAddress);
	StopFallingEffect();
}

