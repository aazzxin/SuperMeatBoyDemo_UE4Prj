// Fill out your copyright notice in the Description page of Project Settings.

#include "FDataManager.h"
#include "Paths.h"
#include "FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "PlatformFile.h"
#include "JsonSerializer.h"
#include "Json.h"

FDataManager::FDataManager()
{
	Load();
	Save(DataFile, DemoJson);
}

FDataManager::~FDataManager()
{
}

FDataManager* FDataManager::Instance;

FDataManager & FDataManager::Get()
{
	if (!Instance) {
		Instance = new FDataManager();
	}
	return *Instance;
}

void FDataManager::Save()
{
	DemoJson.Get()->SetObjectField(PassField, PassJson);
	Save(DataFile, DemoJson);
}

void FDataManager::Load()
{
	FileDirectoryPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	DemoJson = LoadData(DataFile);

	const TSharedPtr<FJsonObject>* PassMapJsonPtr;
	if (DemoJson->TryGetObjectField(PassField, PassMapJsonPtr))
		PassJson = *PassMapJsonPtr;
	else
		PassJson = MakeShareable(new FJsonObject());

	const TSharedPtr<FJsonObject>* LevelNameJsonPtr;
	if (DemoJson->TryGetObjectField(LevelNameField, LevelNameJsonPtr))
		LevelNameJson = *LevelNameJsonPtr;
	else
		LevelNameJson = MakeShareable(new FJsonObject());
}

TSharedPtr<FJsonObject> FDataManager::LoadData(FString FileName) {
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FileDirectoryPath)) {
		PlatformFile.CreateDirectory(*FileDirectoryPath);
		if (!PlatformFile.DirectoryExists(*FileDirectoryPath))
		{
			return MakeShareable(new FJsonObject());
		}
	}
	FString AbsoluteFilePath = FileDirectoryPath + "/" + FileName;

	// Allow overwriting or file doesn't already exist
	if (PlatformFile.FileExists(*AbsoluteFilePath)) {
		FString ResultString;
		FFileHelper::LoadFileToString(ResultString, *AbsoluteFilePath);
		TSharedPtr<FJsonObject> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResultString);

		bool bFlag = FJsonSerializer::Deserialize(JsonReader, JsonParsed);
		if (bFlag) {
			return JsonParsed;
		}
	}
	return MakeShareable(new FJsonObject());
	//FFileHelper::SaveStringToFile("", *AbsoluteFilePath);
	/*if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteSourcePath))
	{
		return;
	}*/
}

void FDataManager::Save(FString FileName, TSharedPtr<FJsonObject> JsonPtr) {
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*FileDirectoryPath)) {
		PlatformFile.CreateDirectory(*FileDirectoryPath);
		if (!PlatformFile.DirectoryExists(*FileDirectoryPath))
		{
			return;
		}
	}
	FString AbsoluteFilePath = FileDirectoryPath + "/" + FileName;

	FString OutputString;
	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&OutputString);
	if (FJsonSerializer::Serialize(JsonPtr.ToSharedRef(), Writer))
		FFileHelper::SaveStringToFile(OutputString, *AbsoluteFilePath);
}