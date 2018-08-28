// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Image.h"
#include "Button.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "MenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMEATBOY_API UMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Meta = (BindWidget))
    UVerticalBox * SelectButton;
	UPROPERTY(Meta = (BindWidget))
	UImage* Arrow;
	UPROPERTY(Meta = (BindWidget))
    UTextBlock* TextStart;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* TextExit;
    UButton* ButtonStart;
	UButton* ButtonExit;

	UFUNCTION(BlueprintCallable, Category = "WidgetFunction")
	void OnButtonHover(UButton* Button);
	UFUNCTION(BlueprintCallable, Category = "WidgetFunction")
	void OnButtonUnhover(UButton* Button);

protected:
	virtual void NativeConstruct() override;


private:
	UFUNCTION(BlueprintCallable, Category = "WidgetFunction")
	void OnStartGame();
	UFUNCTION(BlueprintCallable, Category = "WidgetFunction")
	void OnExitGame();

	float TextHeight;
};
