// Fill out your copyright notice in the Description page of Project Settings.


#include "MPickupItemBase.h"
#include "UEtestGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMPickupItemBase::AMPickupItemBase():SpawnHeight(50.0f), CoolDownTime(10.0f), bActive(true)
{
	RootComponent = MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("PickupItem");
	bReplicates = true;
}

void AMPickupItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(DoSthtoPawn(InstigatorPawn))
	{
		bActive = false;
		OnRep_Active();
		if (HasAuthority())
		{
			GetWorldTimerManager().SetTimer(CoolDownTimer, this, &AMPickupItemBase::ReadyForSpawn, CoolDownTime);
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, TEXT("Item Interacted"));
			AUEtestGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AUEtestGameModeBase>();
			if (GameMode)
			{
				GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, TEXT("待移除Item :") + this->GetActorLocation().ToCompactString());
				if (!GameMode->ItemsLocation.RemoveSingleSwap(this->GetActorLocation()))
				{
					GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("ItemLocation移除失败"));
				}
			}
		}
	}
	
}

void AMPickupItemBase::GetInteractText_Implementation(APawn* InstigatorPawn, FText& Message, FText& AdditionalMessage)
{
	Message = AdditionalMessage = FText::GetEmpty();
}

bool AMPickupItemBase::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	return true;
}

void AMPickupItemBase::BeginPlay()
{
	Super::BeginPlay();
}


void AMPickupItemBase::ReadyForSpawn()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	QueryInstance->SetNamedParam("MinSpawnDist", GetWorld()->GetAuthGameMode<AUEtestGameModeBase>()->MinSpawnDist);
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
		AUEtestGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AUEtestGameModeBase>();
		if (GameMode)
		{
			GameMode->ItemsLocation.Emplace(SpawnLocations[0]);
		}
	}
	bActive = true;
	OnRep_Active();
}


void AMPickupItemBase::OnRep_Active()
{
	SetActorEnableCollision(bActive);
	SetActorHiddenInGame(!bActive);
}

void AMPickupItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMPickupItemBase, bActive);
}