// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacManGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEGAMES_API APacManGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	int iHighScore = 0;
};
