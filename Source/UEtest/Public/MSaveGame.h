// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MSaveGame.generated.h"

/**
 * 
 */
USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
public:
	FActorSaveData() = default;
	FActorSaveData(FName ActorNameToSave, FTransform ActorTransformToSave, TArray<uint8> ActorDataToSave) :ActorName(ActorNameToSave), ActorTransform(ActorTransformToSave)
	{
		ActorData = MoveTemp(ActorDataToSave);
	}
	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ActorData;
};

UCLASS()
class UETEST_API UMSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()  //缺少宏则无法被序列化
	float Credits;

	UPROPERTY()
	TArray<FActorSaveData> ActorSaved;
};
