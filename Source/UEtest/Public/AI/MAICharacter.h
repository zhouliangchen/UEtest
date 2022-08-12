// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "MAICharacter.generated.h"

class UPawnSensingComponent;
class UMAttributeComponent;

UCLASS()
class UETEST_API AMAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocket;

// Sets default values for this character's properties
	AMAICharacter();
	static int GetBotNum();
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMAttributeComponent* AttributeComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName HitTimeParam;//材质参数名，用于输入时间，将差值刷新至0，从而触发HitFlash

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsLowHealth;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float HealthThreshold;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	bool SetTargetActor(AActor* TargetActor);
	virtual void PostInitializeComponents()override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	inline static int ExistBotNum = 0;
};
