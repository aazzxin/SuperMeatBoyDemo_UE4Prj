// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelActor.h"
#include "TerminalActor.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API ATerminalActor : public ALevelActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	FORCEINLINE void SetLevelToLoad(int LevelName) { LevelToLoad = LevelName; }
protected:
	virtual void OverlapAction(ASuperMeatBoyCharacter* Character); 
	int LevelToLoad;
};
