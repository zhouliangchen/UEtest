// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "UEtestGameModeBase.generated.h"

class AMPickupItemBase;
class AMAICharacter;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;

UCLASS()
class UETEST_API AUEtestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	//用于防止Items生成过于密集，可用时返回true
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsSpawnValid(const FVector& PickedLocation);
	//目前已生成的Items位置
	UPROPERTY(BlueprintReadOnly,Category="Gameplay")
	TArray<FVector> ItemsLocation;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	double MinSpawnDist;
	AUEtestGameModeBase();
	virtual void StartPlay() override;
	UFUNCTION(Exec)
	void killAllEnemies();

	virtual void OnPlayerKilled(AActor* VictimActor, AActor* InstigatorActor);
	virtual void OnMinionKilled(AActor* VictimActor, AActor* InstigatorActor);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<AMPickupItemBase>> PickupItemClasses;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int32 SpawnItemsNum;
	UPROPERTY(EditDefaultsOnly)
	float PlayerSpawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float MinionReward;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float MinionEnergyReward;

	FTimerHandle SpawnBotTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float IntervalTime;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UEnvQuery* SpawnItemsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AMAICharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UFUNCTION()
	void OnBotSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UFUNCTION()
	void OnItemsSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	void SpawnBot_Delay();
	void OnPlayerKilled_Delay(AController* Controller);

};
