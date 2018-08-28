// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckPosActor.h"

void ACheckPosActor::OverlapAction(ASuperMeatBoyCharacter* Character) {
	if (Character) {
		UE_LOG(LogTemp, Error, TEXT("Check New Point!"));
		Character->SetCheckPoint(Collision->K2_GetComponentLocation());
	}
}

