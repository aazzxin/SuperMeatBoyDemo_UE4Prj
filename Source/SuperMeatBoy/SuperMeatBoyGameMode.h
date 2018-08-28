// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JsonObject.h"
#include "FDataManager.h"
#include "SuperMeatBoyGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEventDelegate);

UENUM(BlueprintType)
enum class GameModeState :uint8 {
	Menu,
	LevelSelect,
	Play
};

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of SuperMeatBoyCharacter
 */
UCLASS(minimalapi)
class ASuperMeatBoyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASuperMeatBoyGameMode();
	void BeginPlay() override;

	FGameEventDelegate GameWinDelegate;

	void SwitchState(GameModeState State);
	void HideLevelWidgetInstace();
	void HideMenuWidgetInstance();


	int CurLevel; // 当前加载的关卡
	FORCEINLINE GameModeState GetState() { return CurState; }
	FORCEINLINE void SetCheckPos(FVector pos) { CheckPos = pos;
	UE_LOG(LogTemp, Error, TEXT("CheckPos:%s"), *GetCheckPos().ToString());
	}
	FORCEINLINE FVector GetCheckPos() { return CheckPos; }
	void ReBorn(); // 角色重生

	void EscapeScene(); // 玩家按下ESC，返回前一个场景
	float LevelBoundXMin, LevelBoundXMax;
	float LevelBoundZMin, LevelBoundZMax;
	float TileWidth;
	void SetBound();

	FORCEINLINE int GetTotalLevelCount() { return TotalLevelCount; }
	FORCEINLINE FString GetLevelName(FName LevelName) { 
		FString ResultName;
		FDataManager::Get().GetLevelName()->TryGetStringField(LevelName.ToString(), ResultName);
		return ResultName;
	}
	FORCEINLINE bool GetIsPass(FName LevelName) { 
		//if (PassMap.Contains(LevelName)) { return PassMap[LevelName]; } else return false; 
		bool isPass = false;
		FDataManager::Get().PassJson->TryGetBoolField(LevelName.ToString(), isPass);
		return isPass;
	}
	FORCEINLINE void SetPass() { 
		if (PassCount == GetTotalLevelCount())
			return;
		//PassMap.Add(FName(*FString::Printf(TEXT("Forest_1_%d"), CurLevel)), true); PassCount++; 
		FDataManager::Get().PassJson->SetBoolField(FString::Printf(TEXT("Forest_1_%d"), CurLevel), true);
		FDataManager::Get().Save();
		PassCount++;
	}
	FORCEINLINE int GetPassCount() { return PassCount; }
protected:
	/** The widget class we will use as our game over screen when the player wins. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> LevelWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
		TSubclassOf<UUserWidget> MenuWidgetClass;
	/** The widget instance that we are using as our menu. */
	UPROPERTY()
		UUserWidget*  LevelWidgetInstance;
	UPROPERTY()
		UUserWidget* MenuWidgetInstace;
	// 创建widget对象到Viewport
	void CreateWidgetInstace();
	GameModeState CurState;

	int PassCount;
private:
	static const int TotalLevelCount = 8;

	FVector CheckPos;

	void StableCharacter(bool Stable);
	UFUNCTION()
	void ShowWin();
};
