// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3BlueprintFunctionLibrary.h"
#include "Match3GameModeBase.h"
#include "Kismet/GameplayStatics.h"


APlayerController* UMatch3BlueprintFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && PlayerController->IsLocalController())
			{
				// For this project, we will only ever have one local player.
				return PlayerController;
			}
		}
	}
	return nullptr;
}

FString UMatch3BlueprintFunctionLibrary::GetOnlineAccountID(APlayerController* PlayerController)
{
	/*if (PlayerController && PlayerController->PlayerState && PlayerController->PlayerState->UniqueId.IsValid())
	{
		return PlayerController->PlayerState->UniqueId->GetHexEncodedString();
	}*/
	return FString();
}

bool UMatch3BlueprintFunctionLibrary::IsGameActive(UObject* WorldContextObject)
{
	if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		if (GameMode->IsGameActive())
		{
			return true;
		}
	}
	return false;
}

void UMatch3BlueprintFunctionLibrary::PauseGameTimer(UObject* WorldContextObject, bool bPause)
{
	if (AMatch3GameModeBase* GameMode = Cast<AMatch3GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		GameMode->PauseGameTimer(bPause);
	}
}