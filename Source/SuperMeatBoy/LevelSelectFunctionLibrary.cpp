// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelectFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine.h"

APlayerController* ULevelSelectFunctionLibrary::GetLocalPlayerController(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController->IsLocalController())
			{
				// For this project, we will only ever have one local player.  
				return PlayerController;
			}
		}
	}
	return nullptr;
}