// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperGrid.h"
#include "GameFramework/Actor.h"
#include "MineField.generated.h"

UCLASS()
class ARCADEGAMES_API AMineField : public AActor
{
	GENERATED_BODY()
	
public:	
	AMineField();

protected:
	virtual void BeginPlay() override;

public:	
	AMinesweeperGrid* Grid = nullptr;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void OpenField();

	void ShowMine();
	void ShowMineCount(int i);

	void SetMine(bool b) { bIsMine = b; }
	bool IsMine() { return bIsMine; }

private:
	bool bIsMine;
	bool bIsActive;

	USceneComponent* SceneComponent;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Base;

	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mine;

	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num01;

	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num02;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num03;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num04;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num05;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num06;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num07;
	
	UPROPERTY(Category = Mine, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Num08;
};
