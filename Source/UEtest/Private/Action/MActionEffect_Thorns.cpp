// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MActionEffect_Thorns.h"

#include "MAttributeComponent.h"
#include "MGamePlayFunctionLibrary.h"
#include "Action/MActionComponent.h"

UMActionEffect_Thorns::UMActionEffect_Thorns():ReflectRate(0.2f)
{
	Duration = Period = 0.0f;
}


void UMActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(OwnerComp->GetOwner());
	if(AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UMActionEffect_Thorns::ReflectDamage);
	}
}

void UMActionEffect_Thorns::StopAction_Implementation(AActor* Instigator, bool bEmergency)
{
	Super::StopAction_Implementation(Instigator, bEmergency);
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(OwnerComp->GetOwner());
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UMActionEffect_Thorns::ReflectDamage);
	}
}

//���˫������Thorns�⻷���ᷴ�����෴����δ��ͨ�������˺����Ϳ��Խ����bug
//DOT�˺����Կ��ǲ��������Է�ֹ��������������˺�
void UMActionEffect_Thorns::ReflectDamage(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwnerActor = OwnerComp->GetOwner();
	if (Delta >= 0.0f || InstigatorActor == OwnerActor)return;
	if(UMGamePlayFunctionLibrary::ApplyDamage(OwnerActor, InstigatorActor, roundf(-Delta * ReflectRate)))
	{
		UE_LOG(LogTemp, Log, TEXT("Thorns :Deal Reflect Damage from %s to %s"), *GetNameSafe(OwnerActor), *GetNameSafe(InstigatorActor));
	}
}



