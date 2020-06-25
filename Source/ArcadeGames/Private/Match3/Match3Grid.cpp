// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3Grid.h"
#include "Match3GameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Match3BlueprintFunctionLibrary.h"

AMatch3Grid::AMatch3Grid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	MinimumRunLength = 3;
	TileSize.Set(25.0f, 25.0f);
}

void AMatch3Grid::InitGrid()
{
	GameTiles.Empty(GridWidth * GridHeight);
	GameTiles.AddZeroed(GameTiles.Max());
	FVector SpawnLocation;
	for (int32 Column = 0; Column < GridWidth; ++Column)
	{
		for (int32 Row = 0; Row < GridHeight; ++Row)
		{
			int32 TileID = SelectTileFromLibrary();
			int32 GridAddress;
			GetGridAddressWithOffset(0, Column, Row, GridAddress);
			SpawnLocation = GetLocationFromGridAddress(GridAddress);

			do
			{
				TileID = SelectTileFromLibrary();
				if ((Column >= MinimumRunLength - 1) || (Row >= MinimumRunLength - 1))
				{
					int32 TestAddress;
					int32 TileOffset;
					for (int32 Horizontal = 0; Horizontal < 2; ++Horizontal)
					{
						for (TileOffset = 1; TileOffset < MinimumRunLength; ++TileOffset)
						{
							if (!GetGridAddressWithOffset(0, Column - (Horizontal ? TileOffset : 0), Row - (Horizontal ? 0 : TileOffset), TestAddress) || (GetTileFromGridAddress(TestAddress)->TileTypeID != TileID))
							{
								// Not in a matching run, or off the edge of the map, so stop checking this axis.
								break;
							}
						}
						if (TileOffset == MinimumRunLength)
						{
							// We made it through the whole "check for matching run" loop. This tile completes a scoring run. Pick a new tile type and test again.
							break;
						}
					}
					if (TileOffset < MinimumRunLength)
					{
						// We didn't find a matching run in either direction, so we have a valid tile type.
						break;
					}
				}
				else
				{
					// This tile is too close to the edge to be worth checking.
					break;
				}
			} while (true);
			CreateTile(TileLibrary[TileID].TileClass, TileLibrary[TileID].TileMaterial, SpawnLocation, GridAddress, TileID);
		}
	}
}


AMatch3Tile* AMatch3Grid::CreateTile(TSubclassOf<class AMatch3Tile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID)
{
	// If we have set something to spawn:
	if (TileToSpawn)
	{
		checkSlow(TileLibrary.IsValidIndex(TileTypeID));
		// Check for a valid World:
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set the spawn parameters.
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// Tiles never rotate
			FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			// Spawn the tile.
			AMatch3Tile* const NewTile = World->SpawnActor<AMatch3Tile>(TileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			//NewTile->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
			NewTile->TileTypeID = TileTypeID;
			NewTile->Abilities = TileLibrary[TileTypeID].Abilities;
			NewTile->SetTileMaterial(TileMaterial);
			NewTile->SetGridAddress(SpawnGridAddress);
			GameTiles[SpawnGridAddress] = NewTile;
			return NewTile;
		}
	}
	return nullptr;
}

int32 AMatch3Grid::SelectTileFromLibrary()
{
	float NormalizingFactor = 0;
	for (const FTileType& TileBase : TileLibrary)
	{
		NormalizingFactor += TileBase.Probability;
	}
	float TestNumber = FMath::FRandRange(0.0f, NormalizingFactor);
	float CompareTo = 0;
	for (int32 ArrayChecked = 0; ArrayChecked != TileLibrary.Num(); ArrayChecked++)
	{
		CompareTo += TileLibrary[ArrayChecked].Probability;
		if (TestNumber <= CompareTo)
		{
			return ArrayChecked;
		}
	}
	return 0;
}

AMatch3Tile* AMatch3Grid::GetTileFromGridAddress(int32 GridAddress) const
{
	if (GameTiles.IsValidIndex(GridAddress))
	{
		return GameTiles[GridAddress];
	}
	return nullptr;
}

FVector AMatch3Grid::GetLocationFromGridAddress(int32 GridAddress) const
{
	FVector Center = GetActorLocation();
	FVector OutLocation = FVector(-(GridWidth * 0.5f) * TileSize.X + (TileSize.X * 0.5f), 0.0f, -(GridHeight * 0.5f) * TileSize.Y + (TileSize.Y * 0.5f));
	check(GridWidth > 0);
	OutLocation.X += TileSize.X * (float)(GridAddress % GridWidth);
	OutLocation.Z += TileSize.Y * (float)(GridAddress / GridWidth);
	OutLocation += Center;

	return OutLocation;
}

FVector AMatch3Grid::GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const
{
	FVector OutLocation = GetLocationFromGridAddress(GridAddress);
	OutLocation.X += TileSize.X * (float)(XOffsetInTiles);
	OutLocation.Z += TileSize.Y * (float)(YOffsetInTiles);
	return OutLocation;
}

bool AMatch3Grid::GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const
{
	int32 NewAxisValue;

	// Initialize to an invalid address.
	ReturnGridAddress = -1;

	// Check for going off the map in the X direction.
	check(GridWidth > 0);
	NewAxisValue = (InitialGridAddress % GridWidth) + XOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridWidth - 1)))
	{
		return false;
	}

	// Check for going off the map in the Y direction.
	NewAxisValue = (InitialGridAddress / GridWidth) + YOffset;
	if (NewAxisValue != FMath::Clamp(NewAxisValue, 0, (GridHeight - 1)))
	{
		return false;
	}

	ReturnGridAddress = (InitialGridAddress + XOffset + (YOffset * GridWidth));
	check(ReturnGridAddress >= 0);
	check(ReturnGridAddress < (GridWidth * GridHeight));
	return true;
}

bool AMatch3Grid::AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const
{
	if ((FMath::Min(GridAddressA, GridAddressB) >= 0) && (FMath::Max(GridAddressA, GridAddressB) < (GridWidth * GridHeight)))
	{
		int32 GridAddressOffset = FMath::Abs(GridAddressA - GridAddressB);
		return ((GridAddressOffset == 1) || (GridAddressOffset == GridWidth));
	}
	return false;
}

void AMatch3Grid::OnTileFinishedFalling(AMatch3Tile* Tile, int32 LandingGridAddress)
{
	int32 ReturnGridAddress;

	// Remove the tile from its original position if it's still there (hasn't been replaced by another falling tile).
	if (GetGridAddressWithOffset(Tile->GetGridAddress(), 0, 0, ReturnGridAddress))
	{
		if (GameTiles[ReturnGridAddress] == Tile)
		{
			GameTiles[ReturnGridAddress] = nullptr;
		}
	}

	// Validate new grid address and replace whatever is there.
	if (GetGridAddressWithOffset(LandingGridAddress, 0, 0, ReturnGridAddress))
	{
		GameTiles[ReturnGridAddress] = Tile;
		Tile->SetGridAddress(ReturnGridAddress);
		Tile->TileState = ETileState::eNormal;
	}

	// This tile is no longer falling, remove it from the list.
	FallingTiles.RemoveSingleSwap(Tile);
	TilesToCheck.Add(Tile);
	if (FallingTiles.Num() == 0)
	{
		// Done with all falling tiles. Spawn new ones at the top of each column in the appropriate quantity.
		RespawnTiles();
	}
}

void AMatch3Grid::OnTileFinishedMatching(AMatch3Tile* InTile)
{
	if (InTile)
	{
		TilesBeingDestroyed.RemoveSwap(InTile);
		InTile->Destroy();
	}
	if (TilesBeingDestroyed.Num() == 0)
	{
		// Make all tiles fall if they are above empty space.
		for (AMatch3Tile* Tile : FallingTiles)
		{
			Tile->StartFalling();
		}

		if (FallingTiles.Num() == 0)
		{
			RespawnTiles();
		}
	}
}

void AMatch3Grid::RespawnTiles()
{
	for (int32 x = 0; x < GridWidth; ++x)
	{
		// Replace all null tiles, starting from the top of the column. Stop when we hit a non-null tile.
		int32 BaseAddress, TestAddress;
		if (GetGridAddressWithOffset(0, x, GridHeight - 1, BaseAddress))
		{
			int32 y_depth;
			for (y_depth = 0; GetGridAddressWithOffset(BaseAddress, 0, -y_depth, TestAddress) && (!GetTileFromGridAddress(TestAddress)); ++y_depth)
			{
				// This loop finds the lowest Y value, but does nothing with it.
			}
			for (int32 y = y_depth - 1; y >= 0; --y)
			{
				int32 NewTileTypeID = SelectTileFromLibrary();
				GetGridAddressWithOffset(BaseAddress, 0, -y, TestAddress);
				// Move our tile up visually so it has room to fall, but don't change its grid address. The new grid address would be off-grid and invalid anyway.
				if (AMatch3Tile* NewTile = CreateTile(TileLibrary[NewTileTypeID].TileClass, TileLibrary[NewTileTypeID].TileMaterial, GetLocationFromGridAddressWithOffset(TestAddress, 0, (y_depth + 1)), TestAddress, NewTileTypeID))
				{
					TilesToCheck.Add(NewTile);
					NewTile->TileState = ETileState::eFalling;
					check(!FallingTiles.Contains(NewTile));
					FallingTiles.Add(NewTile);
				}
			}
		}
		else
		{
			check(false);
		}
	}

	if (FallingTiles.Num() > 0)
	{
		// Any falling tiles that exist at this point are new ones, and are falling from physical locations (off-grid) to their correct locations.
		for (AMatch3Tile* Tile : FallingTiles)
		{
			Tile->StartFalling(true);
		}
		return;
	}

	// Check to see if any matches have been made automatically.
	TArray<AMatch3Tile*> AllMatchingTiles;
	for (AMatch3Tile* Tile : TilesToCheck)
	{
		TArray<AMatch3Tile*> MatchingTiles = FindNeighbors(Tile);
		for (AMatch3Tile* MatchingTile : MatchingTiles)
		{
			AllMatchingTiles.AddUnique(MatchingTile);
		}
	}

	if (AllMatchingTiles.Num() > 0)
	{
		SetLastMove(EMoveType::eCombo);
		ExecuteMatch(AllMatchingTiles);
	}
	else
	{
		if (IsUnwinnable())
		{
			if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				GameMode->GameOver();
				return;
			}
		}
		UMatch3BlueprintFunctionLibrary::PauseGameTimer(this, false);
	}
}

void AMatch3Grid::SwapTiles(AMatch3Tile* A, AMatch3Tile* B, bool bRepositionTileActors /* = false */)
{
	// Swap grid positions for A and B
	int32 GridAddress = A->GetGridAddress();
	A->SetGridAddress(B->GetGridAddress());
	B->SetGridAddress(GridAddress);

	// Swap array positions for A and B
	GameTiles[A->GetGridAddress()] = A;
	GameTiles[B->GetGridAddress()] = B;

	if (bRepositionTileActors)
	{
		// Move tiles to their new positions
		A->SetActorLocation(GetLocationFromGridAddress(A->GetGridAddress()));
		B->SetActorLocation(GetLocationFromGridAddress(B->GetGridAddress()));
	}
}

TArray<AMatch3Tile*> AMatch3Grid::GetExplosionList(AMatch3Tile* A) const
{
	check(A);
	check(A->Abilities.CanExplode());
	int32 AdjustedBombPower = A->Abilities.BombPower;
	if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(A)))
	{
		AdjustedBombPower = FMath::Max(1, AdjustedBombPower + 1 + GameMode->CalculateBombPower());
	}
	return FindNeighbors(A, false, AdjustedBombPower);
}

bool AMatch3Grid::IsMoveLegal(AMatch3Tile* A, AMatch3Tile* B)
{
	if (A && B && (A != B) && A->Abilities.CanSwap() && B->Abilities.CanSwap())
	{
		if ((A->TileTypeID != B->TileTypeID) && AreAddressesNeighbors(A->GetGridAddress(), B->GetGridAddress()))
		{
			// Swap tiles A and B.
			SwapTiles(A, B);

			// Check for matches with A and B in their proposed positions.
			LastLegalMatch = FindNeighbors(A);
			LastLegalMatch.Append(FindNeighbors(B));

			// Restore positions for A and B.
			SwapTiles(A, B);

			return (LastLegalMatch.Num() > 0);
		}
	}
	return false;
}

TArray<AMatch3Tile*> AMatch3Grid::FindNeighbors(AMatch3Tile* StartingTile, bool bMustMatchID /* = true */, int32 RunLength /* = MinimumRunLength */) const
{
	int32 NeighborGridAddress;
	AMatch3Tile* NeighborTile;
	TArray<AMatch3Tile*> MatchInProgress;
	TArray<AMatch3Tile*> AllMatchingTiles;

	if (RunLength < 0)
	{
		RunLength = MinimumRunLength;
	}

	// Handle special, trivial cases.
	if (RunLength == 0)
	{
		return AllMatchingTiles;
	}
	else if (RunLength == 1)
	{
		AllMatchingTiles.Add(StartingTile);
		return AllMatchingTiles;
	}

	// Check verticals, then check horizontals.
	for (int32 Horizontal = 0; Horizontal < 2; ++Horizontal)
	{
		// Check negative direction, then check positive direction.
		for (int32 Direction = -1; Direction <= 1; Direction += 2)
		{
			int32 MaxGridOffset = !bMustMatchID ? RunLength : (Horizontal ? GridWidth : GridHeight);
			// Check run length. A run ends when we go off the edge of the map or hit a tile that doesn't match, provided we care about matching.
			for (int32 GridOffset = 1; GridOffset < MaxGridOffset; ++GridOffset)
			{
				if (GetGridAddressWithOffset(StartingTile->GetGridAddress(), Direction * (Horizontal ? GridOffset : 0), Direction * (Horizontal ? 0 : GridOffset), NeighborGridAddress))
				{
					NeighborTile = GetTileFromGridAddress(NeighborGridAddress);
					if (NeighborTile && (!bMustMatchID || (NeighborTile->TileTypeID == StartingTile->TileTypeID)))
					{
						MatchInProgress.Add(NeighborTile);
						continue;
					}
					break;
				}
			}
		}
		// See if we have enough to complete a run, or if matching wasn't required. If so, keep the tiles. Note that we add 1 to our match-in-progress because the starting tile isn't counted yet.
		if (!bMustMatchID || ((MatchInProgress.Num() + 1) >= FMath::Min(RunLength, Horizontal ? GridWidth : GridHeight)))
		{
			AllMatchingTiles.Append(MatchInProgress);
		}
		MatchInProgress.Empty();
	}
	// If we found any other tile, or if we're not concerned with matching TileID, then we know we have a valid run, and we need to add the original tile to the list.
	// If we do care about matching tile type and we haven't found anything by this point, then we don't have a match and should not return the starting tile in a list by itself.
	if ((AllMatchingTiles.Num() > 0) || !bMustMatchID)
	{
		AllMatchingTiles.Add(StartingTile);
	}
	return AllMatchingTiles;
}

TArray<AMatch3Tile*> AMatch3Grid::FindTilesOfType(int32 TileTypeID) const
{
	TArray<AMatch3Tile*> ReturnList;
	for (AMatch3Tile* Tile : GameTiles)
	{
		if (Tile && (Tile->TileTypeID == TileTypeID))
		{
			ReturnList.Add(Tile);
		}
	}
	return ReturnList;
}

// We're using a constant array reference for MatchingTiles.
// Constant because we know we'll never change the contents of the array inside this function.
// Reference because we don't need to make a local copy of the array, and it is often better for performance to avoid copying.
void AMatch3Grid::ExecuteMatch(const TArray<AMatch3Tile*>& MatchingTiles)
{
	if (MatchingTiles.Num() == 0)
	{
		return;
	}
	UMatch3BlueprintFunctionLibrary::PauseGameTimer(this, true);
	// Destroy all tiles in MatchingTiles and award points.
	for (AMatch3Tile* Tile : MatchingTiles)
	{
		// Tell all tiles above any tile we're about to destroy that they need to fall. Up on the screen is negative Y on the grid.
		int32 NextAddressUp;
		AMatch3Tile* NextTileUp;
		for (int32 YOffset = 1; YOffset < GridHeight; ++YOffset)
		{
			if (GetGridAddressWithOffset(Tile->GetGridAddress(), 0, YOffset, NextAddressUp))
			{
				NextTileUp = GetTileFromGridAddress(NextAddressUp);
				// If the tile above us is invalid or is being destroyed, stop adding to the list.
				if (NextTileUp && !MatchingTiles.Contains(NextTileUp))
				{
					// Set the tile to falling state as soon as it is added to the list.
					NextTileUp->TileState = ETileState::eFalling;
					check(!FallingTiles.Contains(NextTileUp));
					FallingTiles.Add(NextTileUp);
					continue;
				}
				break;
			}
		}
		Tile->TileState = ETileState::ePendingDelete;
	}

	/** Establish number of tiles we'll need to check after moving tiles and refilling the board. */
	TilesToCheck.Reset(FallingTiles.Num() + MatchingTiles.Num());

	// Add score based on tile count.
	{
		if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			EMoveType MT = GetLastMove();
			int32 ScoreMult = GetScoreMultiplierForMove(MT);
			// Special results for certain move types.
			switch (MT)
			{
			case EMoveType::eBomb:
			case EMoveType::eAllTheBombs:
				// Clear combo when bombing.
				GameMode->SetComboPower(0);
				break;
			case EMoveType::eCombo:
				// Power up combo!
				GameMode->SetComboPower(FMath::Min(GameMode->GetMaxComboPower(), GameMode->GetComboPower() + 1));
				break;
			}
			OnMoveMade(MT);
			GameMode->AddScore(MatchingTiles.Num() * ScoreMult);
		}

		for (AMatch3Tile* Tile : MatchingTiles)
		{
			TilesBeingDestroyed.Add(Tile);
			GameTiles[Tile->GetGridAddress()] = nullptr;
			Tile->OnMatched(GetLastMove());
		}
	}

	// Check in case there are no tiles to destroy.
	OnTileFinishedMatching(nullptr);
}

void AMatch3Grid::OnSwapDisplayFinished(AMatch3Tile* Tile)
{
	SwappingTiles.Add(Tile);
	if (SwappingTiles.Num() == 2)
	{
		check(SwappingTiles[0] && SwappingTiles[1]);
		bPendingSwapMove = false;
		if (bPendingSwapMoveSuccess)
		{
			SwapTiles(SwappingTiles[0], SwappingTiles[1], true);
			SwappingTiles.Reset();
			if (LastLegalMatch.Num() > MinimumRunLength)
			{
				SetLastMove(EMoveType::eMoreTiles);
			}
			else
			{
				SetLastMove(EMoveType::eStandard);
			}
			// Execute the (verified legal) move.
			ExecuteMatch(LastLegalMatch);
		}
		else
		{
			SwappingTiles.Empty();
			OnMoveMade(EMoveType::eFailure);
		}
	}
}

void AMatch3Grid::OnTileWasSelected(AMatch3Tile* NewSelectedTile)
{
	// Can't select tiles while tiles are animating/moving, or game is not active.
	if (FallingTiles.Num() || TilesBeingDestroyed.Num() || bPendingSwapMove || !UMatch3BlueprintFunctionLibrary::IsGameActive(this) || !NewSelectedTile)
	{
		return;
	}

	FTileType& NewSelectedTileType = TileLibrary[NewSelectedTile->TileTypeID];
	if (CurrentlySelectedTile)
	{
		// Selecting a neighbor results in attempting a move.
		if (AreAddressesNeighbors(CurrentlySelectedTile->GetGridAddress(), NewSelectedTile->GetGridAddress()))
		{
			if (NewSelectedTileType.Abilities.CanSwap())
			{
				bPendingSwapMove = true;
				bPendingSwapMoveSuccess = (IsMoveLegal(CurrentlySelectedTile, NewSelectedTile));
				CurrentlySelectedTile->OnSwapMove(NewSelectedTile, bPendingSwapMoveSuccess);
				NewSelectedTile->OnSwapMove(CurrentlySelectedTile, bPendingSwapMoveSuccess);
			}
			else
			{
				// Indicate failure because the second tile was not movable. Deselect the tile we were trying to swap with it.
				OnMoveMade(EMoveType::eFailure);
			}
		}

		// Whatever happened with the selected tiles, they are no longer selected.
		CurrentlySelectedTile->PlaySelectionEffect(false);
		CurrentlySelectedTile = nullptr;
	}
	else
	{
		// Check for various special abilities on the (single) selected tile.
		if (NewSelectedTileType.Abilities.CanExplode())
		{
			TArray<AMatch3Tile*> TilesToDestroy;

			if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				if (GameMode->GetComboPower() == GameMode->GetMaxComboPower())
				{
					// Detonate all bombs at once!
					SetLastMove(EMoveType::eAllTheBombs);
					// If we had multiple bomb types, this would only find the type of bomb we clicked on, because we're matching by checking TileTypeID instead of bCanExplode.
					TArray<AMatch3Tile*> Bombs = FindTilesOfType(NewSelectedTile->TileTypeID);
					TArray<AMatch3Tile*> TilesToDestroyForCurrentBomb;
					for (AMatch3Tile* Bomb : Bombs)
					{
						TilesToDestroyForCurrentBomb = GetExplosionList(Bomb);
						for (AMatch3Tile* TileToCheck : TilesToDestroyForCurrentBomb)
						{
							// Don't add tiles that are already covered by a different bomb.
							TilesToDestroy.AddUnique(TileToCheck);
						}
					}
				}
			}
			if (TilesToDestroy.Num() == 0)
			{
				// Regular bomb detonation, need to establish a list of tiles to destroy.
				SetLastMove(EMoveType::eBomb);
				TilesToDestroy = GetExplosionList(NewSelectedTile);
			}
			ExecuteMatch(TilesToDestroy);
		}
		else if (NewSelectedTileType.Abilities.CanSwap())
		{
			// This is the first tile in the sequence, so remember it for later.
			CurrentlySelectedTile = NewSelectedTile;
			CurrentlySelectedTile->PlaySelectionEffect(true);
		}
		else
		{
			// Indicate failure because the first tile had no usable abilities.
			OnMoveMade(EMoveType::eFailure);
		}
	}
}

bool AMatch3Grid::IsUnwinnable()
{
	for (AMatch3Tile* Tile : GameTiles)
	{
		check(Tile);
		int32 TileGridAddress = Tile->GetGridAddress();
		int32 NeighborGridAddress;
		// Bombs are always valid.
		if (Tile->Abilities.CanExplode())
		{
			return false;
		}
		// If any tile can move in any direction, then the game is not unwinnable.
		if (GetGridAddressWithOffset(TileGridAddress, 0, -1, NeighborGridAddress) && IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
		{
			return false;
		}
		if (GetGridAddressWithOffset(TileGridAddress, 0, 1, NeighborGridAddress) && IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
		{
			return false;
		}
		if (GetGridAddressWithOffset(TileGridAddress, -1, 0, NeighborGridAddress) && IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
		{
			return false;
		}
		if (GetGridAddressWithOffset(TileGridAddress, 1, 0, NeighborGridAddress) && IsMoveLegal(Tile, GetTileFromGridAddress(NeighborGridAddress)))
		{
			return false;
		}
	}
	// No powerups or other non-tile moves are available, and no tiles can move in any direction.
	return true;
}

void AMatch3Grid::SetLastMove(EMoveType MoveType)
{
	if (APlayerController* PC = UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this))
	{
		// Find (or add) the entry for this PlayerController and set it to the type of move that was just made.
		// This is primarily useful for multiplayer games, but will work in single-player as well.
		EMoveType& LastMoveType = LastMoves.FindOrAdd(PC);
		LastMoveType = MoveType;
	}
}

EMoveType AMatch3Grid::GetLastMove()
{
	// Retrieve the type of move most recently made by the given player.
	// This could be stored as a single variable instead of a TMap if we were certain that our game would never support multiplayer.
	if (APlayerController* PC = UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(this))
	{
		if (EMoveType* MoveType = LastMoves.Find(PC))
		{
			return *MoveType;
		}
	}
	return EMoveType::eNone;
}

int32 AMatch3Grid::GetScoreMultiplierForMove_Implementation(EMoveType LastMoveType)
{
	// Default value of 100 points per action.
	return 100;
}

void AMatch3Grid::ReturnMatchSounds(TArray<USoundWave*>& MatchSounds)
{
	MatchSounds.Reset();
	if (TilesBeingDestroyed.Num() > 0)
	{
		for (AMatch3Tile* Tile : TilesBeingDestroyed)
		{
			MatchSounds.AddUnique(Tile->GetMatchSound());
		}
	}
}
