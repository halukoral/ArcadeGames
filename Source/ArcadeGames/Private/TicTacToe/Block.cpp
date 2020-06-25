// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "BlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> X;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> O;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("StaticMesh'/Game/TicTacToe/Meshes/PuzzleCube.PuzzleCube'"))
			, X(TEXT("StaticMesh'/Game/TicTacToe/Meshes/SM_X.SM_X'"))
			, O(TEXT("StaticMesh'/Game/TicTacToe/Meshes/SM_O.SM_O'"))
			, BaseMaterial(TEXT("Material'/Game/TicTacToe/Materials/BaseMaterial.BaseMaterial'"))
			, BlueMaterial(TEXT("MaterialInstanceConstant'/Game/TicTacToe/Materials/BlueMaterial.BlueMaterial'"))
			, OrangeMaterial(TEXT("MaterialInstanceConstant'/Game/TicTacToe/Materials/OrangeMaterial.OrangeMaterial'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ABlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ABlock::OnFingerPressedBlock);

	PlaceHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceHolder"));
	PlaceHolder->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	PlaceHolder->SetupAttachment(DummyRoot);

	X = (ConstructorStatics.X.Get());
	O = (ConstructorStatics.O.Get());


	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
}

void ABlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void ABlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void ABlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		if (OwningGrid != nullptr)
		{
			if (OwningGrid->IsX())
			{
				PlaceHolder->SetStaticMesh(X);
				BlockState = EBlockState::T_P2;
			}
			else
			{
				PlaceHolder->SetStaticMesh(O);
				BlockState = EBlockState::T_P1;
			}


			OwningGrid->SetIsX(!OwningGrid->IsX());
			OwningGrid->OnMoveEnds(BlockState);
		}
	}
}

void ABlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void ABlock::Restart()
{
	bIsActive = false;
	PlaceHolder->SetStaticMesh(nullptr);
	BlockState = EBlockState::T_None;
	BlockMesh->SetMaterial(0, BlueMaterial);
}

