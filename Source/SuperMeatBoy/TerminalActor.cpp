// Fill out your copyright notice in the Description page of Project Settings.

#include "TerminalActor.h"
#include "Engine/World.h"
#include "SuperMeatBoyGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATerminalActor::BeginPlay(){
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Current Level Name : %s"),*GetWorld()->GetMapName());
}

void ATerminalActor::OverlapAction(ASuperMeatBoyCharacter* Character) {
	UE_LOG(LogTemp, Warning, TEXT("LoadLevel"));
		//LogLevel: ActivateLevel /Game/Maps/UEDPIE_0_Forest4 1 1 1
	    ASuperMeatBoyGameMode*  GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode) {
			FLatentActionInfo LatenInfo;
			FName LoadName;
			if (LevelToLoad == -1) {
				LoadName = TEXT("GameWin");
			}
			else
				LoadName = FName(*FString::Printf(TEXT("Forest_1_%d"), LevelToLoad));
			UGameplayStatics::LoadStreamLevel(this, LoadName, true, true, LatenInfo);

			GameMode->SetPass();
		}
}

