// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEtestGameModeBase.h"

#include "EngineUtils.h"
#include "MAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/Character.h"

AUEtestGameModeBase::AUEtestGameModeBase():IntervalTime(2.0f)
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

void AUEtestGameModeBase::SpawnBot_Delay()
{
	
	int BotNum = EnemyClass.GetDefaultObject()->GetBotNum();
	float MaxNum = 3.0f;
	//实现AI数量控制，数量可通过遍历Actor得到，也可自行在AI class中维护static变量，性能更好
	//BotNum = 0;
	// for(ACharacter* Character:TActorRange<ACharacter>(GetWorld(),EnemyClass))
	// {
	// 	++BotNum;
	// }
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
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();
	if(SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AMAICharacter>(EnemyClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}



