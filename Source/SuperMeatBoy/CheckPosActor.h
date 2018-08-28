// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelActor.h"
#include "CheckPosActor.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API ACheckPosActor : public ALevelActor
{
	GENERATED_BODY()
	
protected:
	virtual void OverlapAction(ASuperMeatBoyCharacter* Character) override;
	
	
};
