// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TetrisBlock.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisGrid.generated.h"

class UChildActorComponent;

UCLASS()
class ARCADEGAMES_API ATetrisGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetrisGrid();

protected:
	virtual void	BeginPlay() override;
	virtual void	PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	float			AdditionalTime() { return fAdditionalTimeOnGround; }
	float			GameSpeed() { return fGameSpeed; }
	int				GetTileSize() { return iTileSize; }
	
	void			LevelUp();
	void			GameOver();
	void			PoolTetrisBlock(ATetrisBlock* Block);
	void			IncreasePoints(ETetrisScore e);
	void			SpawnNewBlock();
	void			StartGame(bool bStarted);
	void			RemoveFullRows();
	void			TetrisBlockReachedGround();
	void			DecreaseRows(int index);
	void			DeleteRow(int index);
	bool			IsRowFull(int index);
	bool			IsValidIndex(TArray<FVector2D> indexes, TArray<FVector2D> indexesNew);
	ATetrisBlock*	GetNewTetrisBlock(FVector v, ETetrisBlock b);
	
	UFUNCTION(BlueprintCallable)
	void			GenerateGrid();

	UFUNCTION(BlueprintPure)
	FVector			ConvertGridIndexToWorldLocation(FVector2D index);

	UFUNCTION(BlueprintPure)
	FVector2D		ConvertWorldLocationToGridIndex(FVector index);

	ATetrisBlock*	GetActiveBlock() { return pActiveBlock; }
	
	void			UpdateTetrisShadows();
	void			UpdateTetrisBlocks();

	UPROPERTY(Category = Grid, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FVector2D, UStaticMeshComponent*> GridMap;

private:
	int								iCurrentLevel;
	int								iCurrentCombo;
	int								iCurrentPoints;
	int								iCurrentCompletedRowCount;
	int								iMaxLevel = 10;

	float							fGameSpeed = 1.f;
	const float						fAdditionalTimeOnGround = 0.25;

	ETetrisScore					LastScore;

	USceneComponent*				SceneComponent;
	ATetrisBlock*					pActiveBlock = nullptr;
	TArray<ETetrisBlock>			eTetrisBlocks;
	TArray<ATetrisBlock*>			SpawnedBlocks;
	TArray<ATetrisBlock*>			PooledBlocks;
	
	TArray<UChildActorComponent*>	TetrisBlocks;
	UChildActorComponent*			TetrisShadow;

	UPROPERTY(Category = Block, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATetrisBlock>		TetrisBlockClass;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iWidth = 5;
	
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iHeight = 15;
	
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iTileSize = 100;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iBlockCount = 6;

};
