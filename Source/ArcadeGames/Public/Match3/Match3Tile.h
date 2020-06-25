// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3Tile.generated.h"


class USoundWave;
class AMatch3Grid;

UENUM()
enum class ETileState : uint8
{
	eNormal,
	eFalling,
	ePendingDelete
};

UENUM(BlueprintType)
enum class EMoveType : uint8
{
	eNone,
	eFailure,
	eStandard,
	eMoreTiles,
	eCombo,
	eBomb,
	eAllTheBombs,
	eMAX
};


USTRUCT(BlueprintType)
struct FTileAbilities
{
	GENERATED_USTRUCT_BODY();

	bool CanExplode() { return bExplodes; }
	bool CanSwap() { return (!bPreventSwapping && !bExplodes); }

protected:
	/** Tile explodes when selected (change this!) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool	bExplodes = true;

	/** Tile can't be selected as part of a normal swapping move. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool	bPreventSwapping = true;

public:
	/** Power rating of a bomb. What this means is determined in GameMode code, and can consider what kind of bomb this is. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32	BombPower = 0;

	// Replace this when bitfields can have default member initializers
	FTileAbilities() : bExplodes(false) , bPreventSwapping(false) { }
};

UCLASS()
class ARCADEGAMES_API AMatch3Tile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMatch3Tile();
	
	void		BeginPlay() override;

	/* ------------- INPUT ------------- */

	/** When a tile is touched. */
	UFUNCTION()
	void		TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	/** When the user's finger moves over a tile. */
	UFUNCTION()
	void		TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	/** Mouse surrogate for TilePress. */
	UFUNCTION()
	void		TilePress_Mouse(AActor* ClickedActor, FKey ButtonClicked);

	/** Mouse surrogate for TileEnter. */
	UFUNCTION()
	void		TileEnter_Mouse(AActor* MousedOverActor);


	/* ------------- EFFECTS ------------- */

	UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Events")
	void		PlaySelectionEffect(bool bTurnEffectOn);

	UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Events")
	void		StartFallingEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Events")
	void		StopFallingEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Special Game Events")
	void		SetTileMaterial(class UMaterialInstanceConstant* TileMaterial);
	

	/* ------------- EVENTS ------------- */

	/** Called when a match has been made, and says what type of move led to the match. */
	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Events")
	void		OnMatched(EMoveType MoveType);
	void		OnMatched_Implementation(EMoveType MoveType);

	UFUNCTION(BlueprintNativeEvent, Category = "Special Game Events")
	void		OnSwapMove(AMatch3Tile* OtherTile, bool bMoveWillSucceed);
	void		OnSwapMove_Implementation(AMatch3Tile* OtherTile, bool bMoveWillSucceed);

	UFUNCTION()
	void		TickFalling();
	void		StartFalling(bool bUseCurrentWorldLocation = false);
	void		FinishFalling();

	void		SetGridAddress(int32 NewLocation) { GridAddress = NewLocation; }
	int32		GetGridAddress() const { return GridAddress; }

	USoundWave* GetMatchSound() { return MatchSound; }

	UPROPERTY()
	TEnumAsByte<ETileState> TileState;

	UPROPERTY(BlueprintReadOnly)
	FTileAbilities			Abilities;

	UPROPERTY(BlueprintReadOnly)
	int32					TileTypeID;

protected:
	float					TotalFallingTime;
	float					FallingStartTime;
	FVector					FallingStartLocation;
	FVector					FallingEndLocation;
	FTimerHandle			TickFallingHandle;

	/** Location on the grid as a 1D key/value. To find neighbors, ask the grid. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32					GridAddress;

	/** Location where we will land on the grid as a 1D key/value. Used while falling. */
	int32					LandingGridAddress;

	/** The grid that owns this tile. Currently, this is set by casting the object that spawned the tile. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	AMatch3Grid*			Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, Meta = (BlueprintProtected = True))
	USoundWave*				MatchSound;
};
