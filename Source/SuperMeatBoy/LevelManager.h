// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "TerminalActor.h"
#include "LevelManager.generated.h"

UCLASS()
class SUPERMEATBOY_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void EnterLevel();

private:
	UPROPERTY(EditAnywhere, Category = LavelMap)
	APaperTileMapActor * Map;
	float LevelBoundXMin, LevelBoundXMax;
	float LevelBoundZMin, LevelBoundZMax;

	UPROPERTY(EditAnywhere, Category = LavelMap)
	ATerminalActor* Terminal;
	UPROPERTY(EditAnywhere, Category = LavelMap)
	int LevelToLoad;
};
