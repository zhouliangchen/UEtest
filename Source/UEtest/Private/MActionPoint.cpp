// Fill out your copyright notice in the Description page of Project Settings.


#include "MActionPoint.h"

#include "Action/MAction.h"
#include "Action/MActionComponent.h"

bool AMActionPoint::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	if (UMActionComponent* ActionComp = UMActionComponent::GetActionComp(InstigatorPawn))
	{
		if (ensure(ActionClass) && ActionComp->AddActionUnique(ActionClass, this))
		{
			return true;
		}
	}
	return false;
}
