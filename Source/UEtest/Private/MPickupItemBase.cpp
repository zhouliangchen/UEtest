// Fill out your copyright notice in the Description page of Project Settings.


#include "MPickupItemBase.h"

// Sets default values
AMPickupItemBase::AMPickupItemBase():CoolDownTime(10.0f)
{
	RootComponent = MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

}

void AMPickupItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	if(DoSthtoPawn(InstigatorPawn))
	{
		GetWorldTimerManager().SetTimer(CoolDownTimer, this, &AMPickupItemBase::ReadyForUse,CoolDownTime);
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
	
}

bool AMPickupItemBase::DoSthtoPawn_Implementation(APawn* InstigatorPawn)
{
	return true;
}

void AMPickupItemBase::ReadyForUse()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}


