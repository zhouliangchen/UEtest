// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "MPickupItemBase.generated.h"

UCLASS(Abstract)
class UETEST_API AMPickupItemBase : public AActor,public IMGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPickupItemBase();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	FTimerHandle CoolDownTimer;
	float CoolDownTime;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool DoSthtoPawn(APawn* InstigatorPawn);
	virtual bool DoSthtoPawn_Implementation(APawn* InstigatorPawn);
	void ReadyForUse();
public:	

};
