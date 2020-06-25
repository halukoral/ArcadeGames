// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Block.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "BlockGrid.generated.h"

UCLASS()
class ARCADEGAMES_API ABlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent*		DummyRoot;

	class UTextRenderComponent* Player1Tooltip;

	class UTextRenderComponent* Player2Tooltip;

	class UTextRenderComponent* Player1Wins;

	class UTextRenderComponent* Player2Wins;

	class UTextRenderComponent* GameState;

public:
	ABlockGrid();

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, VisibleAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	bool IsX() { return bIsX; }
	void SetIsX(bool NweValue) { bIsX = NweValue; }
	
	bool IsOwner(EBlockState PlayerSide, int32 BlockIndex);

	bool CheckWin(EBlockState PlayerSide);

	void OnMoveEnds(EBlockState PlayerSide);

	void OnRefreshMoveText();

	void GameOver();

protected:
	virtual void BeginPlay() override;

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

private:
	bool bIsX = false;

	TArray<ABlock*> BlockArray;

	uint8 P1Score = 0;
	uint8 P2Score = 0;
	uint8 EmptyBlock = 0;

};
