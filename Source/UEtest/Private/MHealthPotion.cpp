// Fill out your copyright notice in the Description page of Project Settings.


#include "MHealthPotion.h"
#include "MAttributeComponent.h"

AMHealthPotion::AMHealthPotion():PowerUpNumber(50.0f)
{
}

bool AMHealthPotion::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	if(InstigatorPawn)
	{
		UMAttributeComponent* AttributeComp = Cast<UMAttributeComponent>(InstigatorPawn->GetComponentByClass(UMAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			//可以省略的检查
			//if(!AttributeComp->IsFullHealth()&&AttributeComp->IsAlive())
			if(AttributeComp->ApplyHealthChange(PowerUpNumber))
			{
				return true;
			}
		}
	}
	return false;
}
