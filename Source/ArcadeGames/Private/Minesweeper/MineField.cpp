// Fill out your copyright notice in the Description page of Project Settings.


#include "MineField.h"

AMineField::AMineField()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneComponent; 
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(SceneComponent);
	Base->OnClicked.AddDynamic(this, &AMineField::BlockClicked);
	Base->OnInputTouchBegin.AddDynamic(this, &AMineField::OnFingerPressedBlock);

	Mine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mine"));
	Mine->SetupAttachment(SceneComponent);
	Mine->SetVisibility(false);

	Num01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num01"));
	Num01->SetupAttachment(SceneComponent);
	
	Num02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num02"));
	Num02->SetupAttachment(SceneComponent);
	
	Num03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num03"));
	Num03->SetupAttachment(SceneComponent);
	
	Num04 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num04"));
	Num04->SetupAttachment(SceneComponent);
	
	Num05 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num05"));
	Num05->SetupAttachment(SceneComponent);
	
	Num06 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num06"));
	Num06->SetupAttachment(SceneComponent);
	
	Num07 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num07"));
	Num07->SetupAttachment(SceneComponent);
	
	Num08 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Num08"));
	Num08->SetupAttachment(SceneComponent);

	Num01->SetVisibility(false);
	Num02->SetVisibility(false);
	Num03->SetVisibility(false);
	Num04->SetVisibility(false);
	Num05->SetVisibility(false);
	Num06->SetVisibility(false);
	Num07->SetVisibility(false);
	Num08->SetVisibility(false);
}

void AMineField::BeginPlay()
{
	Super::BeginPlay();
	bIsActive = false;
}

void AMineField::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OpenField();
}

void AMineField::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OpenField();
}

void AMineField::OpenField()
{
	// Check we are not already active
	if (!bIsActive)
	{
		if (Grid != nullptr)
		{
			if (bIsMine)
			{
				Grid->ShowAllMines();
			}
			else
			{
				int Count = Grid->MineCountOnNeighbours(this);
				if (Count > 0)
				{
					ShowMineCount( Count );
				}
				else
				{
					bIsActive = true;
					Base->SetVisibility(false);
					Grid->FindAllEmptyNeighbours(this);
				}
			}
		}
	}
}

void AMineField::ShowMine()
{
	if (bIsMine)
	{
		bIsActive = true;
		Base->SetVisibility(false);
		Mine->SetVisibility(true);
	}
}

void AMineField::ShowMineCount(int i)
{
	bIsActive = true;
	Base->SetVisibility(false);
	if (i == 1)
		Num01->SetVisibility(true);
	if (i == 2)				  
		Num02->SetVisibility(true);
	if (i == 3)				  
		Num03->SetVisibility(true);
	if (i == 4)				  
		Num04->SetVisibility(true);
	if (i == 5)				  
		Num05->SetVisibility(true);
	if (i == 6)				  
		Num06->SetVisibility(true);
	if (i == 7)				  
		Num07->SetVisibility(true);
	if (i == 8)				  
		Num08->SetVisibility(true);
}

