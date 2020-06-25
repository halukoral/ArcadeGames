// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/GameViewportClient.h"
#include "LocalMultiplayer.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEGAMES_API ULocalMultiplayer : public UGameViewportClient
{
	GENERATED_BODY()

public:
	/*virtual bool InputKey(
		FViewport* Viewport, 
		int32 ControllerId, 
		FKey Key, 
		EInputEvent EventType, 
		float AmountDepressed = 1.f, 
		bool bGamepad = false
	) override;	*/
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs);
};
