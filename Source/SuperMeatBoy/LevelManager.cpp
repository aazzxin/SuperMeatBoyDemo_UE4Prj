// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "SuperMeatBoyGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	EnterLevel();
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelManager::EnterLevel()
{
	ASuperMeatBoyGameMode*  GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		if (Map) {
			FVector LeftTopPos = Map->GetRenderComponent()->GetTileCenterPosition(0, 0, 0, true);
			FVector Extent = Map->GetRenderComponent()->TileMap->GetRenderBounds().BoxExtent;
			Extent *= 2;
			LevelBoundXMin = LeftTopPos.X;
			LevelBoundXMax = LevelBoundXMin + Extent.X - Map->GetRenderComponent()->TileMap->TileWidth;
			LevelBoundZMax = LeftTopPos.Z;
			LevelBoundZMin = LevelBoundZMax - Extent.Z;
			GameMode->LevelBoundXMin = LevelBoundXMin;
			GameMode->LevelBoundXMax = LevelBoundXMax;
			GameMode->LevelBoundZMax = LevelBoundZMax;
			GameMode->LevelBoundZMin = LevelBoundZMin;
			GameMode->TileWidth = Map->GetRenderComponent()->TileMap->TileWidth;
			GameMode->SetBound();
			GameMode->SwitchState(GameModeState::Play);
		}
		if (RootComponent) {
			GameMode->SetCheckPos(GetActorLocation());
			GameMode->ReBorn();
		}
		if (Terminal)
			Terminal->SetLevelToLoad(LevelToLoad);

		FLatentActionInfo LatenInfo;
		FName UnloadName;
		UE_LOG(LogTemp, Warning, TEXT("Current Level:%d"), GameMode->CurLevel);
		if (GameMode->CurLevel == -1)
			UnloadName = TEXT("GameWin");
		else if (GameMode->CurLevel == GameMode->GetTotalLevelCount()) {
			UnloadName = FName(*FString::Printf(TEXT("Forest_1_%d"), GameMode->GetTotalLevelCount()));
			GameMode->GameWinDelegate.Broadcast();
		}
		else
			UnloadName = FName(*FString::Printf(TEXT("Forest_1_%d"), GameMode->CurLevel));
		UGameplayStatics::UnloadStreamLevel(this, UnloadName, LatenInfo);

		GameMode->CurLevel = LevelToLoad == -1 ? GameMode->GetTotalLevelCount() : LevelToLoad - 1;
	}
}

