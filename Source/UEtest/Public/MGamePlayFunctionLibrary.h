// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MGamePlayFunctionLibrary.generated.h"

class AMPlayerController;
/**
 * 
 */
UCLASS()
class UETEST_API UMGamePlayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//造成伤害，仅当伤害生效时返回true
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount);
	//造成带冲击力的伤害，仅当伤害生效时返回true（与冲击力是否生效无关）
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);
	//对玩家的Credits进行修改，修改成功时返回true
	
};
