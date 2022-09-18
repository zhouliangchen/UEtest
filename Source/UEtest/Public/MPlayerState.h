// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerState.generated.h"

class UMSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreditsReplicatedSignature, float, NewCredits);
/**
 * 
 */
UCLASS()
class UETEST_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool UpdatePlayerCredits(float Delta);
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	float GetPlayerCredits()const;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsReplicatedSignature OnCreditsReplicated;
	AMPlayerState();
	UFUNCTION(BlueprintNativeEvent,Category="SaveGame")
	void SavePlayerState(UMSaveGame* SaveGame);
	UFUNCTION(BlueprintNativeEvent, Category = "SaveGame")
	void LoadPlayerState(UMSaveGame* SaveGame);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_Credits", Category = "Gameplay")
	float Credits;
	UFUNCTION()
	void OnRep_Credits();
};
