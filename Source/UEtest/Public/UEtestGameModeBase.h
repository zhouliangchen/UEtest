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
	UFUNCTION(Exec)
	void killAllEnemies();
	
	virtual void OnPlayerKilled(AActor* VictimActor, AActor* InstigatorActor);
	virtual void OnMinionKilled(AActor* VictimActor, AActor* InstigatorActor);
protected:
	UPROPERTY(EditDefaultsOnly)
	float PlayerSpawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "GamePlay")
	float MinionReward;

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
	void OnPlayerKilled_Delay(AController* Controller);

};
