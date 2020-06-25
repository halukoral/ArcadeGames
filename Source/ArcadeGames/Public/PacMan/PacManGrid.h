// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fruit.h"
#include "Pellet.h"
#include "PowerfulPellet.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacManGrid.generated.h"

UENUM(BlueprintType)
enum class ECellType : uint8
{
	None,
	Pellet,
	PowerfulPellet,
	pBlock,
	pFruit
};

UCLASS()
class ARCADEGAMES_API APacManGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	APacManGrid();

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void		GenerateGrid();

	UFUNCTION(BlueprintPure)
	FVector		ConvertGridIndexToWorldLocation(FVector2D index);

	UFUNCTION(BlueprintPure)
	FVector2D	ConvertWorldLocationToGridIndex(FVector index);

	UPROPERTY(Category = Grid, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FVector2D, UStaticMeshComponent*> GridMap;

private:
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int						iWidth = 20;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int						iHeight = 20;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int						iTileSize = 100;

	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ECellType				eCell = ECellType::None;

	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFruit*					pFruit = nullptr;
	
	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APellet*				pPellet = nullptr;
	
	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APowerfulPellet*		pPowerfulPellet = nullptr;
	
	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent*	pBlock = nullptr; 
	
	UPROPERTY(Category = Cell, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMesh*			pMesh = nullptr;

	USceneComponent*		SceneComponent = nullptr;
};
