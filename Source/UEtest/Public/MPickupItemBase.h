// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameplayInterface.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Actor.h"
#include "MPickupItemBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;

UCLASS(Abstract)
class UETEST_API AMPickupItemBase : public AActor,public IMGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPickupItemBase();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float SpawnHeight;
protected:
	FTimerHandle CoolDownTimer;
	float CoolDownTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UEnvQuery* SpawnItemsQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool DoSthtoPawn(APawn* InstigatorPawn);

	virtual bool DoSthtoPawn_Implementation(APawn* InstigatorPawn);
	virtual void BeginPlay() override;
	void ReadyForSpawn();
	UFUNCTION()
	void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
public:	

};
