// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "StackSpawner.generated.h"

UCLASS()
class ARCADEGAMES_API AStackSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	
	FVector SetColor();
	FVector GetColor();
	void SpawnTile();
	void MoveTile(float deltaTime);
	void EndGame();
	bool IntersectionRight(float& remainderScaleY);
	bool IntersectionLeft(float& remainderScaleX);
	void RemainderRight(bool bCanSpawn, float remainderScaleY);
	void RemainderLeft(bool bCanSpawn, float remainderScaleX);


public:	
	// Sets default values for this actor's properties
	AStackSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StackFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpeedUpCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOnLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATile *CurrentTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATile* PrevTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float H = 165.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float S = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float V = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurrentColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent *LeftArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent *RightArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATile> ReminderClass;

	int iCount = 0;
};
