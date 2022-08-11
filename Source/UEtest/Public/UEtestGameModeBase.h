// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/MAICharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "UEtestGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;

UCLASS()
class UETEST_API AUEtestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AUEtestGameModeBase();
	virtual void StartPlay() override;
protected:
	FTimerHandle SpawnBotTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float IntervalTime;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AMAICharacter> EnemyClass;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	void SpawnBot_Delay();

};
