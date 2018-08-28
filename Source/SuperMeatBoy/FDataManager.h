// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObject.h"

/**
 * 
 */
class SUPERMEATBOY_API FDataManager
{
public:
	FDataManager();
	~FDataManager();

	static FDataManager& Get();

	TSharedPtr<FJsonObject> PassJson; // 地图是否通过

	FORCEINLINE TSharedPtr<FJsonObject> GetLevelName() const { return LevelNameJson; }

	void Save();
	void Load();

private:
	static FDataManager* Instance;

	const FString DataFile = "DemoData";
	const FString PassField = "PassMap";
	const FString LevelNameField = "LevelName";
	
	TSharedPtr<FJsonObject> DemoJson;

	TSharedPtr<FJsonObject> LevelNameJson; // 每个关卡对应的名字

    FString FileDirectoryPath;
	TSharedPtr<FJsonObject> LoadData(FString FileName);
	void Save(FString FileName, TSharedPtr<FJsonObject> JsonPtr);
};
