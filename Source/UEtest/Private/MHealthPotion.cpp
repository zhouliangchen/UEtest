// Fill out your copyright notice in the Description page of Project Settings.


#include "MHealthPotion.h"
#include "MAttributeComponent.h"

AMHealthPotion::AMHealthPotion():PowerUpNumber(50.0f)
{
}

bool AMHealthPotion::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(InstigatorPawn);
	if (AttributeComp)
	{
		//����ʡ�Եļ��
		//if(!AttributeComp->IsFullHealth()&&AttributeComp->IsAlive())
		if (AttributeComp->ApplyHealthChange(this, PowerUpNumber))
		{
			return true;
		}
	}
	return false;
}
