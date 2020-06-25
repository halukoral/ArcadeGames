// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinesweeperGrid.generated.h"

class AMineField;

UCLASS()
class ARCADEGAMES_API AMinesweeperGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AMinesweeperGrid();

protected:
	virtual void	BeginPlay() override;

public:	
	void			ShowAllMines();

	void			IsMine(int x, int y);

	void			FindAllEmptyNeighbours(AMineField* MineField);

	int				MineCountOnNeighbours(AMineField* MineField);

	UFUNCTION(BlueprintCallable)
	void			GenerateGrid();

	UFUNCTION(BlueprintPure)
	FVector			ConvertGridIndexToWorldLocation(FVector2D index);

	UFUNCTION(BlueprintPure)
	FVector2D		ConvertWorldLocationToGridIndex(FVector Loc);

private:
	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iWidth = 5;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iHeight = 5;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int iTileSize = 100;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMineField> MineFieldClass;

	UPROPERTY(Category = Grid, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FVector2D, AMineField*> GridMap;

	TArray<AMineField*> MineFields;
};
