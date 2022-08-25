// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeComponent.h"

#include "GameFramework/Character.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("mt.DamageMultiplier"), 1.0f, TEXT("设置全局伤害倍率"), ECVF_Cheat);
// Sets default values for this component's properties
UMAttributeComponent::UMAttributeComponent():Health(100.0f),HealthMax(100.0f),Energy(0.0f),EnergyMax(100.0f), DamageConvertionRate(0.2f), AttackConvertionRate(0.5f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;
	// ...
}

UMAttributeComponent* UMAttributeComponent::GetAttributeComp(AActor* Actor)
{
	if(Actor)
	{
		return Cast<UMAttributeComponent>(Actor->GetComponentByClass(UMAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UMAttributeComponent::IsActorAlive(AActor* Actor)
{
	UMAttributeComponent* AttributeComp = GetAttributeComp(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}


// // Called every frame
// void UMAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	GEngine->AddOnScreenDebugMessage(1, 1.0, FColor::Yellow, FString::SanitizeFloat(Health));
// }

//在血量变动时返回true，并触发OnHealthChanged事件
bool UMAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	if(Delta<0.0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	Delta = NewHealth - Health;
	//Energy=FMath::Clamp(Energy+Delta* EnergyMultiplier,0.0f,)
	if(Delta<0.0f)
	{
		ApplyEnergyChange(InstigatorActor, -Delta * DamageConvertionRate);
		if(ACharacter* Character=Cast<ACharacter>(InstigatorActor))
		{
			if(UMAttributeComponent* AttributeComp = GetAttributeComp(Character))
			{
				AttributeComp->ApplyEnergyChange(GetOwner(), -Delta * AttackConvertionRate);
			}
		}
	}
	Health = NewHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	if (!Health && Delta < 0.0f)
	{
		OnActorKilled.ExecuteIfBound(GetOwner(), InstigatorActor);
	}
	//如果要为每个Actor启用伤害弹窗，应该在这添加
	return Delta ? true : false;
}

bool UMAttributeComponent::ApplyEnergyChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f && Energy + Delta < 0.0f)
	{
		return false;
	}
	float NewEnergy = FMath::Clamp(Energy + Delta, 0.0f, EnergyMax);
	Delta = NewEnergy - Energy;
	Energy = NewEnergy;
	if(Delta)
	{
		OnEnergyChanged.Broadcast(InstigatorActor, this, Energy, Delta);
	}
	return Delta ? true : false;
}

bool UMAttributeComponent::IsEnergyEnough(float EnergyNeeded)const
{
	return Energy >= EnergyNeeded;
}

bool UMAttributeComponent::IsAlive()const
{
	return Health > 0.0f;
}

bool UMAttributeComponent::IsFullHealth() const
{
	//目前还没有出现小数位上的变动，所以直接以==比较浮点数
	return Health == HealthMax;
}

float UMAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UMAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

