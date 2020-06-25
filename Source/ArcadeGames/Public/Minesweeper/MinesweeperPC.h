// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MinesweeperPC.generated.h"

UCLASS()
class ARCADEGAMES_API AMinesweeperPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMinesweeperPC()
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableTouchEvents = true;
		DefaultMouseCursor = EMouseCursor::Crosshairs;
	}
};
