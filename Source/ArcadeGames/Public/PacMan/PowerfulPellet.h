// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacMan/Pellet.h"
#include "PowerfulPellet.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEGAMES_API APowerfulPellet : public APellet
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	APowerfulPellet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;

private:
	TArray<AActor*> Enemies;
};
