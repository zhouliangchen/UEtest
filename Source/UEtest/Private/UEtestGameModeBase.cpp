// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEtestGameModeBase.h"

#include "EngineUtils.h"
#include "MAttributeComponent.h"
#include "MCharacter.h"
#include "MGamePlayFunctionLibrary.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "MPlayerController.h"
#include "MPlayerState.h"
#include "AI/MAICharacter.h"
#include "MPickupItemBase.h"
#include "MSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBot(TEXT("mt.SpawnBot"), false, TEXT("控制AI Bot的自动生成"), ECVF_Cheat);


AUEtestGameModeBase::AUEtestGameModeBase():MinSpawnDist(1200.0), ExistBotNum(0), SpawnItemsNum(10), PlayerSpawnDelay(5.0f),
											MinionReward(10.0f), MinionEnergyReward(20.0f),IntervalTime(2.0f),SaveSlotName("SaveGame01")
{
	PrimaryActorTick.bCanEverTick = true;
}



void AUEtestGameModeBase::StartPlay()
{
	Super::StartPlay();
	if(!ItemsLocation.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("游戏开始，ItemsLocation不为空"));
	}
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, TEXT("游戏开始，当前AI数（应为0）：") + FString::FromInt(EnemyClass.GetDefaultObject()->GetBotNum()));
	GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AUEtestGameModeBase::SpawnBot_Delay, IntervalTime, true, 0);
	if(!PickupItemClasses.IsEmpty())
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		QueryInstance->SetNamedParam("MinSpawnDist", MinSpawnDist);
		if (ensureMsgf(QueryInstance, TEXT("EQS执行出错，请确保参数已正确赋值")))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddUniqueDynamic(this, &AUEtestGameModeBase::OnItemsSpawnQueryFinished);
		}
	}
}

void AUEtestGameModeBase::SpawnBot_Delay()
{
	if(!CVarSpawnBot.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("AI Bots生成已被ConsoleVariable'CVarSpawnBot'禁用"));
		return;
	}
	//int BotNum = EnemyClass.GetDefaultObject()->GetBotNum();
	float MaxNum = 3.0f;
	if(DifficultyCurve)
	{
		MaxNum = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if(ExistBotNum +1>MaxNum)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Blue, TEXT("准备进行AI生成，预计生成后AI数：") + FString::FromInt(ExistBotNum +1) + TEXT("允许AI数：") + FString::SanitizeFloat(MaxNum));
	//EnemyClass.GetDefaultObject()->GetClass()
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensureMsgf(QueryInstance, TEXT("EQS执行出错，请确保参数已正确赋值")))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddUniqueDynamic(this, &AUEtestGameModeBase::OnBotSpawnQueryFinished);
	}
}

void AUEtestGameModeBase::OnBotSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!= EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("SpawnBotQuery事件返回失败响应"));
		return;
	}
	TArray<FVector> SpawnLocations;
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	if(SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AMAICharacter>(EnemyClass, SpawnLocations[0], FRotator::ZeroRotator);
		++ExistBotNum;
	}
}

void AUEtestGameModeBase::OnItemsSpawnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("SpawnItemsQuery事件返回失败响应"));
		return;
	}
	TArray<FVector> SpawnLocations;
	QueryInstance->GetQueryResultsAsLocations(SpawnLocations);
	while (ItemsLocation.Num() < SpawnItemsNum && !SpawnLocations.IsEmpty())
	{
		int32 RandLocationIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		FVector& PickedLocation = SpawnLocations[RandLocationIndex];
		if(IsSpawnValid(PickedLocation))
		{
			int32 RandClassIndex = FMath::RandRange(0, PickupItemClasses.Num() - 1);
			TSubclassOf<AMPickupItemBase> PickedClass = PickupItemClasses[RandClassIndex];
			PickedLocation[2] += PickedClass.GetDefaultObject()->SpawnHeight;
			GetWorld()->SpawnActor<AMPickupItemBase>(PickedClass, PickedLocation, FRotator::ZeroRotator);
			ItemsLocation.Emplace(PickedLocation);
		}
		SpawnLocations.RemoveAtSwap(RandLocationIndex);
	}
	
}
bool AUEtestGameModeBase::IsSpawnValid(const FVector& PickedLocation)
{
	for(FVector &ItemLocation:ItemsLocation)
	{
		if (FVector::DistSquared(PickedLocation, ItemLocation) <= MinSpawnDist* MinSpawnDist)
		{
			return false;
		}
	}
	return true;
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

void AUEtestGameModeBase::OnPlayerKilled_Delay(AController* Controller)
{
	if(Controller)
	{
		if(ACharacter* Character=Controller->GetCharacter())
		{
			Character->Destroy();
			RestartPlayer(Controller);
		}
		
	}
}

void AUEtestGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	for(auto& Test: ItemsLocation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, Test.ToCompactString());
	}
}

void AUEtestGameModeBase::OnMinionKilled(AActor* VictimActor, AActor* InstigatorActor)
{
	VictimActor->SetLifeSpan(5.0f);
	--ExistBotNum;
	AMCharacter* Player = Cast<AMCharacter>(InstigatorActor);
	if(Player)
	{
		Player->GetPlayerState<AMPlayerState>()->UpdatePlayerCredits(MinionReward);
		UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(Player);
		AttributeComp->ApplyEnergyChange(VictimActor, MinionEnergyReward);
	}
}


void AUEtestGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	if(UGameplayStatics::HasOption(Options, "continue"))
	{
		LoadSaveGame();
		IsContinue = true;
	}
	else
	{
		SaveGame = Cast<UMSaveGame>(UGameplayStatics::CreateSaveGameObject(UMSaveGame::StaticClass()));
		if (SaveGame)
		{
			UE_LOG(LogTemp, Log, TEXT("New SaveGame Created"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Create New SaveGame"));
		}
	}
}

void AUEtestGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsContinue)
	{
		if (AMPlayerState* PlayerState = NewPlayer->GetPlayerState<AMPlayerState>())
		{
			PlayerState->LoadPlayerState(SaveGame);
		}
	}
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}


void AUEtestGameModeBase::WriteSaveGame()
{
	for(auto& PS:  GameState->PlayerArray)
	{
		if(AMPlayerState* PlayerState=Cast<AMPlayerState>(PS))
		{
			PlayerState->SavePlayerState(SaveGame);
			break;
		}
	}
	SaveGame->ActorSaved.Empty();
	for(AActor* Actor: FActorRange(GetWorld()))
	{
		if(Actor->Implements<UMGameplayInterface>())
		{
			TArray<uint8> ActorData;
			FMemoryWriter MemWriter(ActorData,true);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			Ar.ArIsSaveGame = true;
			Ar.ArNoDelta = false;
			Actor->Serialize(Ar);
			SaveGame->ActorSaved.Emplace(Actor->GetFName(), Actor->GetActorTransform(), ActorData);
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, 0);
}

void AUEtestGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGame = Cast<UMSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (SaveGame)
		{
			for (AActor* Actor : FActorRange(GetWorld()))
			{
				if (Actor->Implements<UMGameplayInterface>())
				{
					if (FActorSaveData* TargetData = SaveGame->ActorSaved.FindByPredicate(
						[&Actor](FActorSaveData& ActorSaveData)
						{
							return ActorSaveData.ActorName == Actor->GetFName();
						}))
					{
						Actor->SetActorTransform(TargetData->ActorTransform, false, nullptr, ETeleportType::None);
						FMemoryReader MemReader(TargetData->ActorData, true);
						FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
						Ar.ArIsSaveGame = true;
						Ar.ArNoDelta = false;
						Actor->Serialize(Ar);
						IMGameplayInterface::Execute_OnActorLoaded(Actor);
					}
				}
			}
			UE_LOG(LogTemp, Log, TEXT("SaveGame Loaded Successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Load SaveGame"));
		}
	}
	else
	{
		SaveGame = Cast<UMSaveGame>(UGameplayStatics::CreateSaveGameObject(UMSaveGame::StaticClass()));
		if(SaveGame)
		{
			UE_LOG(LogTemp, Log, TEXT("SaveGame File Not Found, New SaveGame Created"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Create New SaveGame"));
		}
	}
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
