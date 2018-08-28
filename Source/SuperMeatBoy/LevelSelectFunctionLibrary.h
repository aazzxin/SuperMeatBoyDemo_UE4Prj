// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelSelectFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API ULevelSelectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Level_Select")
		static APlayerController* GetLocalPlayerController(UObject* WorldContextObject);
	
	
};
