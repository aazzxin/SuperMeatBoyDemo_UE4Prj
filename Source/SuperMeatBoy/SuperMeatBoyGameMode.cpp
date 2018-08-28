// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SuperMeatBoyGameMode.h"
#include "Engine/World.h"
#include "SuperMeatBoyCharacter.h"
#include "LevelSelectFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelectUserWidget.h"
#include "MenuUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FDataManager.h"

ASuperMeatBoyGameMode::ASuperMeatBoyGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ASuperMeatBoyCharacter::StaticClass();	
}

void ASuperMeatBoyGameMode::BeginPlay() {
	Super::BeginPlay();
	CreateWidgetInstace();
	SwitchState(GameModeState::Menu);
	
	for (int i = 0; i < TotalLevelCount; i++) {
		if (GetIsPass(FName(*FString::Printf(TEXT("Forest_1_%d"), i + 1))))
			PassCount++;
	}

	GameWinDelegate.__Internal_AddDynamic(this, &ASuperMeatBoyGameMode::ShowWin, TEXT("ShowWin"));
}

void ASuperMeatBoyGameMode::SwitchState(GameModeState State)
{
	CurState = State;
	switch (State)
	{
	case GameModeState::Menu:
		StableCharacter(true);
		if (MenuWidgetInstace)
		{
			MenuWidgetInstace->AddToViewport();
		}
		break;
	case GameModeState::LevelSelect:
		StableCharacter(true);
		if (LevelWidgetInstance)
		{
			LevelWidgetInstance->AddToViewport();
		}
		break;
	case GameModeState::Play:
		StableCharacter(false);
		break;
	default:
		break;
	}
}

void ASuperMeatBoyGameMode::ReBorn()
{
	UE_LOG(LogTemp, Error, TEXT("Check Pos:%s"), *CheckPos.ToString());
	ACharacter* Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ASuperMeatBoyCharacter* MeatBoy = CastChecked<ASuperMeatBoyCharacter>(Character);

	if (MeatBoy) {
		MeatBoy->ReBorn(CheckPos);
	}
}

void ASuperMeatBoyGameMode::EscapeScene()
{
	switch (CurState)
	{
	case GameModeState::Menu:
		//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit);
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("QUIT");
		break;
	case GameModeState::LevelSelect:
		SwitchState(GameModeState::Menu);
		break;
	case GameModeState::Play:
		SwitchState(GameModeState::LevelSelect);
		break;
	default:
		break;
	}
}

void ASuperMeatBoyGameMode::SetBound()
{
	ACharacter* Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ASuperMeatBoyCharacter* MeatBoy = CastChecked<ASuperMeatBoyCharacter>(Character);
	if (MeatBoy) {
		MeatBoy->LevelBoundXMax = LevelBoundXMax;
		MeatBoy->LevelBoundXMin = LevelBoundXMin;
		MeatBoy->LevelBoundZMax = LevelBoundZMax;
		MeatBoy->LevelBoundZMin = LevelBoundZMin;
		MeatBoy->TileWidth = TileWidth;
		MeatBoy->SetCameraBound();
	}
}

void ASuperMeatBoyGameMode::CreateWidgetInstace()
{
	if (LevelWidgetInstance) {
		LevelWidgetInstance->RemoveFromViewport();
		LevelWidgetInstance = nullptr;
	}
	if (MenuWidgetInstace) {
		MenuWidgetInstace->RemoveFromViewport();
		MenuWidgetInstace = nullptr;
	}
	if (LevelWidgetClass) {
		if (APlayerController* PC = (ULevelSelectFunctionLibrary::GetLocalPlayerController(this)))
		{
			LevelWidgetInstance = CreateWidget<ULevelSelectUserWidget>(PC, LevelWidgetClass);
		}
	}
	if (MenuWidgetClass) {
		if (APlayerController* PC = (ULevelSelectFunctionLibrary::GetLocalPlayerController(this)))
		{
			MenuWidgetInstace = CreateWidget<UMenuUserWidget>(PC, MenuWidgetClass);
		}
	}
}

void ASuperMeatBoyGameMode::StableCharacter(bool Stable)
{
	ACharacter* Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ASuperMeatBoyCharacter* MeatBoy = CastChecked<ASuperMeatBoyCharacter>(Character);
	if (Stable) {
		//MeatBoy->SetActorLocation(FVector::ZeroVector);
		MeatBoy->SetMobility(false);
		MeatBoy->SetActorHiddenInGame(true);
		MeatBoy->SetCameraWidth(3150.0f);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
	else {
		MeatBoy->SetMobility(true);
		MeatBoy->SetActorHiddenInGame(false);
		MeatBoy->SetCameraWidth(2600.0f);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}
}

void ASuperMeatBoyGameMode::ShowWin()
{
}


void ASuperMeatBoyGameMode::HideLevelWidgetInstace()
{
	if (LevelWidgetInstance)
	{
		LevelWidgetInstance->RemoveFromViewport();
	}
}

void ASuperMeatBoyGameMode::HideMenuWidgetInstance()
{
	if (MenuWidgetInstace)
	{
		MenuWidgetInstace->RemoveFromViewport();
	}
}