// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelectUserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SuperMeatBoyGameMode.h"
#include "Engine.h"
#include "UObject/UObjectGlobals.h"

void ULevelSelectUserWidget::NativeConstruct() {
	GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());

	LevelButtonArray.Add(Level1);
	LevelButtonArray.Add(Level2);
	LevelButtonArray.Add(Level3);
	LevelButtonArray.Add(Level4);
	LevelButtonArray.Add(Level5);
	LevelButtonArray.Add(Level6);
	LevelButtonArray.Add(Level7);
	LevelButtonArray.Add(Level8);
	if (GameMode) {
		int PassCount = GameMode->GetPassCount();
		int TotalCount = GameMode->GetTotalLevelCount();
		float Percentage = (float)PassCount / TotalCount;

		BandagesText->SetText(FText::FromString(FString::Printf(TEXT("BANDAGES: %d-%d"), PassCount, TotalCount)));
		CompleteText->SetText(FText::FromString(FString::Printf(TEXT("COMPLETE: %d%%"), (int)(Percentage * 100))));

		/*FString Path = "";
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
		FButtonStyle Style = FButtonStyle();
		FSlateBrush Brush = FSlateBrush();
		Style.SetNormal(Brush);
		for (int i = 0; i < PassCount; i++) {
			LevelButtonArray[i]->SetStyle(Style);
		}*/
	}

}

void ULevelSelectUserWidget::HandleButtonClick(int num) {
	/*FString s = Cast<UTextBlock>(Button->GetChildAt(0))->Text.ToString();
	int num = FCString::Atoi(*s);*/

	ASuperMeatBoyGameMode* GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());
	FName Name = FName(*FString::Printf(TEXT("Forest_1_%d"), num));
	if (CurSelectLevel == num) {
		FLatentActionInfo LatenInfo;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Forest_1_%d"), num));

		if (GameMode) {
			if (num != 1) {
				// 不是第一关，需要检查前一关是否通过
				if (!GameMode->GetIsPass(FName(*FString::Printf(TEXT("Forest_1_%d"), num - 1))))
					return;
			}
			if (num == GameMode->CurLevel) {
				GameMode->SetBound();
				GameMode->ReBorn();
			}
			else
				UGameplayStatics::LoadStreamLevel(this, Name, true, true, LatenInfo);

			GameMode->SwitchState(GameModeState::Play);
			GameMode->HideLevelWidgetInstace();
			//UGameplayStatics::UnloadStreamLevel(this, "Forest3", LatenInfo);
		}
	}
	else {
		CurSelectLevel = num;
		if (GameMode)
			LevelNameText->SetText(FText::FromString(FString::Printf(TEXT("1-%d %s"), num, *GameMode->GetLevelName(Name))));
	}
}

