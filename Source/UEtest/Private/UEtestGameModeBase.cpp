// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEtestGameModeBase.h"

#include "EngineUtils.h"
#include "MAttributeComponent.h"
#include "MCharacter.h"
#include "MGamePlayFunctionLibrary.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "MPlayerController.h"
#include "AI/MAICharacter.h"
#include "GameFramework/Character.h"

static TAutoConsoleVariable<bool> CVarSpawnBot(TEXT("mt.SpawnBot"), true, TEXT("控制AI Bot的自动生成"), ECVF_Cheat);

AUEtestGameModeBase::AUEtestGameModeBase():PlayerSpawnDelay(5.0f), MinionReward(10.0f), IntervalTime(2.0f)
{
}

void AUEtestGameModeBase::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, TEXT("游戏开始，当前AI数（应为0）：") + FString::FromInt(EnemyClass.GetDefaultObject()->GetBotNum()));
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AUEtestGameModeBase::SpawnBot_Delay, IntervalTime, true, 0);
}

void AUEtestGameModeBase::killAllEnemies()
{
	for(AMAICharacter* Character:TActorRange<AMAICharacter>(GetWorld(),EnemyClass))
	{
		UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(Character);
		if(AttributeComp&&AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}


void AUEtestGameModeBase::OnPlayerKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	AMCharacter* Player = Cast<AMCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle RespawnTimeHandle;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &AUEtestGameModeBase::OnPlayerKilled_Delay, Player->GetController());
		GetWorldTimerManager().SetTimer(RespawnTimeHandle, Delegate, PlayerSpawnDelay, false);
		UE_LOG(LogTemp, Log, TEXT("Player %s is killed by %s"), *GetNameSafe(Player), *GetNameSafe(InstigatorActor));
	}
}

void AUEtestGameModeBase::OnMinionKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	AMCharacter* Player = Cast<AMCharacter>(InstigatorActor);
	AMAICharacter* Minion = Cast<AMAICharacter>(VictimActor);
	if(Player&&Minion)
	{
		UMGamePlayFunctionLibrary::UpdatePlayerCredits(Player->GetController<AMPlayerController>(), MinionReward);
	}
}

void AUEtestGameModeBase::OnPlayerKilled_Delay(AController* Controller)
{
	if(Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void AUEtestGameModeBase::SpawnBot_Delay()
{
	if(!CVarSpawnBot.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("AI Bots生成已被ConsoleVariable'CVarSpawnBot'禁用"));
		return;
	}
	int BotNum = EnemyClass.GetDefaultObject()->GetBotNum();
	float MaxNum = 3.0f;
	if(DifficultyCurve)
	{
		MaxNum = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if(BotNum+1>MaxNum)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, TEXT("准备进行AI生成，当前AI数：") + FString::FromInt(BotNum+1) + TEXT("允许AI数：") + FString::SanitizeFloat(MaxNum));
	//EnemyClass.GetDefaultObject()->GetClass()
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensureMsgf(QueryInstance, TEXT("EQS执行出错，请确保参数已正确赋值")))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddUniqueDynamic(this, &AUEtestGameModeBase::OnQueryFinished);
	}
}

void AUEtestGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!= EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("EQS事件返回失败响应"));
		return;
	}
	TArray<FVector> SpawnLocations;
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	if(SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AMAICharacter>(EnemyClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}



