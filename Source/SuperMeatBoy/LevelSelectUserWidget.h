// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperMeatBoyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "TextBlock.h"
#include "LevelSelectUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API ULevelSelectUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "WidgetFunction")
	void HandleButtonClick(int num);

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* LevelNameText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CompleteText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* BandagesText;
	UPROPERTY(Meta = (BindWidget))
    UButton* Level1;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level2;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level3;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level4;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level5;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level6;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level7;
	UPROPERTY(Meta = (BindWidget))
	UButton* Level8;

protected:
	virtual void NativeConstruct() override;
	ASuperMeatBoyGameMode* GameMode;

	TArray<UButton*> LevelButtonArray;

private:
	int CurSelectLevel;
};
