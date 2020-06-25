// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalMultiplayer.h"




//bool ULocalMultiplayer::InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed /*= 1.f*/, bool bGamepad /*= false*/)
bool ULocalMultiplayer::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if ( IgnoreInput() || EventArgs.IsGamepad() || EventArgs.Key.IsMouseButton())
	{
		return Super::InputKey(EventArgs);
	}
	else
	{
		// Propagate keyboard events to all players
		UEngine* const Engine = GetOuterUEngine();
		int32 const NumPlayers = Engine ? Engine->GetNumGamePlayers(this) : 0;
		bool bRetVal = false;
		for (int32 i = 0; i < NumPlayers; i++)
		{
			bRetVal = Super::InputKey(EventArgs) || bRetVal;
		}

		return bRetVal;
	}
}
