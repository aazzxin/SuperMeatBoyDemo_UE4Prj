// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "GameFramework/SpringArmComponent.h"
#include "ClampSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API UClampSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	float LevelBoundXMin, LevelBoundXMax;
	float LevelBoundZMin, LevelBoundZMax;
protected:
	virtual void BeginPlay() override;

	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;
	

};
