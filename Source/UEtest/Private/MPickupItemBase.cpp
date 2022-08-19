// Fill out your copyright notice in the Description page of Project Settings.


#include "MPickupItemBase.h"

#include "EnvironmentQuery/EnvQueryManager.h"

// Sets default values
AMPickupItemBase::AMPickupItemBase():CoolDownTime(10.0f), SpawnHeight(50.0f)
{
	RootComponent = MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("PickupItem");
}

void AMPickupItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(DoSthtoPawn(InstigatorPawn))
	{
		GetWorldTimerManager().SetTimer(CoolDownTimer, this, &AMPickupItemBase::ReadyForSpawn,CoolDownTime);
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
	
}

bool AMPickupItemBase::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	return true;
}

void AMPickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	ReadyForSpawn();
}


void AMPickupItemBase::ReadyForSpawn()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	if(ensureMsgf(QueryInstance,TEXT("EQS执行出错，请确认参数已正确赋值")))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddUniqueDynamic(this, &AMPickupItemBase::OnQueryFinished);
	}
}

void AMPickupItemBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogEQS, Warning, TEXT("Items生成EQS返回失败响应"));
		return;
	}
	TArray<FVector> SpawnLocations;
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	if(SpawnLocations.IsValidIndex(0))
	{
		SpawnLocations[0][2] += SpawnHeight;
		this->SetActorLocation(SpawnLocations[0]);
	}
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}


