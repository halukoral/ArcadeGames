// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StackSpawn.generated.h"

UCLASS()
class ARCADEGAMES_API AStackSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStackSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};
