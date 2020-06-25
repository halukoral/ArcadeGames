// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisBlock.generated.h"

class ATetrisGrid;

UENUM(BlueprintType)
enum class ETetrisBlock : uint8
{
	I,
	J,
	L,
	O,
	S,
	T,
	Z
};

UENUM(BlueprintType)
enum class ETetrisScore : uint8
{
	None,
	SingleRow,
	DoubleRow,
	TripleRow,
	Tetris,
	BackToBackTetris,
	SoftDrop,
	HardDrop,
	FullClear
};

UCLASS()
class ARCADEGAMES_API ATetrisBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATetrisBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void		 SetBlockTransform(float TileSize);

	void		 SetBlockMesh();
	ETetrisBlock GetBlockMesh()					{ return eShape; }
	
	UStaticMesh* GetBlocksMesh()				{ return BlockMesh; }
	void		 SetBlocksMesh(ETetrisBlock e)	{ eShape = e; }
	
	void		 SetTetrisGrid(ATetrisGrid* g)	{ TetrisGrid = g; }
	void		 SetActive(bool b)				{ bActive = b; }
	void		 SetShadow(bool b)				{ bShadow = b; }

	void		 MoveLeft()						{ Move(FVector2D(-1.f, 0.f)); }
	void		 MoveRight()					{ Move(FVector2D(1.f, 0.f)); }
	void		 MoveDown();
	void		 MoveHardDrop();
	void		 Rotate();
	bool		 Rotate(FRotator Rotation);
	bool		 Move(FVector2D Direction);
	void		 MoveDown(float DeltaTime);


	TArray<FVector2D> GetGridIndex(FVector2D vOffset = FVector2D::ZeroVector);
	USceneComponent*			SceneComponent;
	UStaticMeshComponent*		Block01;
	UStaticMeshComponent*		Block02;
	UStaticMeshComponent*		Block03;
	UStaticMeshComponent*		Block04;
	UMaterialInstanceDynamic*	DynamicMaterial;

private:
	
	bool						bShadow = false;
	bool						bActive = false;
	float						fTimeSinceLastMove = 0.f;

	ATetrisGrid*				TetrisGrid;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ETetrisBlock				eShape;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMesh*				BlockMesh;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface*			TetrisBlockMaterial;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_I;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_J;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_L;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_O;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_S;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_T;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_Z;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor				Color_Shadow = FLinearColor(0.01f,0.01f,0.01,1.0f);
};
