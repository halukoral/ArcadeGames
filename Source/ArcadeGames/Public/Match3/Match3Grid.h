// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Match3Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3Grid.generated.h"

USTRUCT(BlueprintType)
struct FTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceConstant* TileMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMatch3Tile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor EffectColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTileAbilities Abilities;
};

UCLASS()
class ARCADEGAMES_API AMatch3Grid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatch3Grid(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AMatch3Tile*>	GameTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileType>		TileLibrary;

	/** The size of a space on the grid. Does not include borders or spacing between tiles. */
	UPROPERTY(EditAnywhere, Category = Tile)
	FVector2D				TileSize;

	/** Minimum number of matching tiles in a row needed to score. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32					MinimumRunLength;

	/** The width of the grid. Needed to calculate tile positions and neighbors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32					GridWidth;

	/** The height of the grid. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32					GridHeight;

	/** Spawn a tile and associate it with a specific grid address. */
	AMatch3Tile*			CreateTile(TSubclassOf<class AMatch3Tile> TileToSpawn, class UMaterialInstanceConstant* TileMaterial, FVector SpawnLocation, int32 SpawnGridAddress, int32 TileTypeID);

	/** Randomly select a type of tile from the grid's library, using the probability values on the tiles. */
	int32					SelectTileFromLibrary();

	/** Get the pointer to the tile at the specified grid address. */
	AMatch3Tile*			GetTileFromGridAddress(int32 GridAddress) const;

	/** Initialize the tiles on the grid*/
	UFUNCTION(BlueprintCallable, Category = Initialization)
	void					InitGrid();

	/** Play effects when a move is made. Use this to avoid spamming sounds on tiles. */
	UFUNCTION(BlueprintImplementableEvent, meta = (ExpandEnumAsExecs = "MoveType"), Category = Tile)
	void					OnMoveMade(EMoveType MoveType);

	UFUNCTION(BlueprintCallable, Category = Audio)
	void					ReturnMatchSounds(TArray<USoundWave*>& MatchSounds);

	/** Get the world location for a given grid address. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	FVector					GetLocationFromGridAddress(int32 GridAddress) const;
	
	/** Get the world location for a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	FVector					GetLocationFromGridAddressWithOffset(int32 GridAddress, int32 XOffsetInTiles, int32 YOffsetInTiles) const;
	
	/** Get a grid address relative to another grid address. Offset between addresses is measured in tiles. */
	UFUNCTION(BlueprintCallable, Category = Tile)
	bool					GetGridAddressWithOffset(int32 InitialGridAddress, int32 XOffset, int32 YOffset, int32& ReturnGridAddress) const;
	
	/** Determine if two grid addresses are valid and adjacent. */
	bool					AreAddressesNeighbors(int32 GridAddressA, int32 GridAddressB) const;

	void					OnTileFinishedFalling(AMatch3Tile* Tile, int32 LandingGridAddress);
	void					OnTileFinishedMatching(AMatch3Tile* InTile);
	void					OnSwapDisplayFinished(AMatch3Tile* InTile);

	void					RespawnTiles();
	void					SwapTiles(AMatch3Tile* A, AMatch3Tile* B, bool bRepositionTileActors = false);

	/** Tests a move to see if it's permitted. */
	bool					IsMoveLegal(AMatch3Tile* A, AMatch3Tile* B);

	/** Get list of tiles that will be affected by a bomb's explosion. */
	TArray<AMatch3Tile*>	GetExplosionList(AMatch3Tile* A) const;
	
	/** Check for a successful sequence. bMustMatchID can be set to false to ignore matching. MinimumLengthRequired will default to the game's MinimumRunLength setting if negative. */
	TArray<AMatch3Tile*>	FindNeighbors(AMatch3Tile* StartingTile, bool bMustMatchID = true, int32 RunLength = -1) const;
	
	/** Find all tiles of a given type. */
	TArray<AMatch3Tile*>	FindTilesOfType(int32 TileTypeID) const;
	
	/** Execute the result of one or more matches. It is possible, with multiple matches, to have more than one tile type in the array. */
	void					ExecuteMatch(const TArray<AMatch3Tile*>& MatchingTiles);
	
	/** React to a tile being clicked. */
	void					OnTileWasSelected(AMatch3Tile* NewSelectedTile);

	/** Detects unwinnable states. */
	bool					IsUnwinnable();

	/** Establishes the most recent move type for the specified player. */
	void					SetLastMove(EMoveType MoveType);

	/** Tells what type of move the specified player made most recently. */
	EMoveType			GetLastMove();

	/** Gives point value per tile based on move type. Default is 100. */
	UFUNCTION(BlueprintNativeEvent, Category = Game)
	int32					GetScoreMultiplierForMove(EMoveType LastMoveType);
	int32					GetScoreMultiplierForMove_Implementation(EMoveType LastMoveType);
	AMatch3Tile*			GetCurrentlySelectedTile() const { return CurrentlySelectedTile; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tile)
	AMatch3Tile*			CurrentlySelectedTile;

private:
	/** Array of tiles found in the most recent call to IsMoveLegal. */
	TArray<AMatch3Tile*>	LastLegalMatch;
	
	/** Tiles that are currently falling. */
	TArray<AMatch3Tile*>	FallingTiles;
	
	/** Tiles that are currently swapping positions with each other. Should be exactly two of them, or zero. */
	TArray<AMatch3Tile*>	SwappingTiles;
	
	/** After spawning new tiles, which tiles to check for automatic matches. */
	TArray<AMatch3Tile*>	TilesToCheck;
	
	/** Tiles that are currently reacting to being matches. */
	TArray<AMatch3Tile*>	TilesBeingDestroyed;
	
	/** Indicates that we are waiting to complete a swap move. When SwappingTiles is populated by two tiles, we are done. */
	uint32					bPendingSwapMove : 1;
	
	/** Indicates that we are waiting to complete a swap move. When SwappingTiles is populated by two tiles, we are done. */
	uint32					bPendingSwapMoveSuccess : 1;

	/** The type of move last executed by a given player. */
	TMap<APlayerController*, EMoveType> LastMoves;
	
};
