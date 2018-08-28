// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuUserWidget.h"
#include "CanvasPanelSlot.h"
#include "WidgetTransform.h"
#include "SuperMeatBoyGameMode.h"
#include "Engine/World.h"

void UMenuUserWidget::OnButtonHover(UButton * Button)
{
	UE_LOG(LogTemp, Warning, TEXT("Change Slot Position"));
	//UCanvasPanelSlot* ArrowSlot = Cast<UCanvasPanelSlot>(Arrow->Slot);
	//UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(Button->Slot);
	//if(Button->Slot)
	//	UE_LOG(LogTemp, Warning, TEXT("Find Button Slot"));
	//if (ArrowSlot) {
	//	UE_LOG(LogTemp, Warning, TEXT("Find Arrow"));
	//	UE_LOG(LogTemp, Warning, TEXT("ArrowTranslation: %s"), *ArrowSlot->GetPosition().ToString());
	//}
	//if (ButtonSlot)
	//	UE_LOG(LogTemp, Warning, TEXT("Find Button"));
	/*if(ArrowSlot && ButtonSlot) {
		UE_LOG(LogTemp, Warning, TEXT("Change Slot Position"));
		ArrowSlot->SetPosition(ButtonSlot->GetPosition());
	}*/

	Button->SetRenderTransform(FWidgetTransform(FVector2D(10, 0), FVector2D(1,1), FVector2D::ZeroVector, 0));
	//UE_LOG(LogTemp, Warning, TEXT("ArrowTranslation: %d"), Button->RenderTransform.Translation.X);
	//UE_LOG(LogTemp, Warning, TEXT("ArrowPosition: %s"),*ArrowSlot->GetPosition().ToString());
	Arrow->SetRenderTransform(FWidgetTransform(FVector2D(0, SelectButton->GetChildIndex(Button) * TextStart->Font.Size * 1.8f), FVector2D(1, 1), FVector2D::ZeroVector, 0));
	//UE_LOG(LogTemp, Warning, TEXT("ArrowPosition: %s"), *ArrowSlot->GetPosition().ToString());
	//Arrow->RenderTransform.Translation = Button->RenderTransform.Translation;
}

void UMenuUserWidget::OnButtonUnhover(UButton * Button)
{
	Button->SetRenderTransform(FWidgetTransform(FVector2D(0, 0), FVector2D(1, 1), FVector2D::ZeroVector, 0));
}

void UMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/*FIntPoint Point = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
	float scale = Point.Y / 1080.0f;
	UE_LOG(LogTemp, Warning, TEXT("Point: %s"), *Point.ToString());
	TextStart->Font.Size *= scale;
	TextExit->Font.Size *= scale;
    TextHeight = TextStart->Font.Size * scale;
	FSlateFontInfo OriginFont = TextStart->Font;
	FSlateFontInfo FontInfo(OriginFont);
	FontInfo.Size = TextHeight;
	TextStart->SetFont(FontInfo);
	TextStart->SetText(FText::FromString(FString("START GAME")));
	TextExit->SetFont(FontInfo);
	TextExit->SetText(FText::FromString(FString("EXIT GAME")));
	UE_LOG(LogTemp, Warning, TEXT("TextHeight: %f"), TextHeight);*/
}

void UMenuUserWidget::OnStartGame()
{
	ASuperMeatBoyGameMode* GameMode = Cast<ASuperMeatBoyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->SwitchState(GameModeState::LevelSelect);
		GameMode->HideMenuWidgetInstance();
	}
}

void UMenuUserWidget::OnExitGame()
{
	//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit);
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("QUIT");
}
