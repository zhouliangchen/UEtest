// Fill out your copyright notice in the Description page of Project Settings.


#include "MGamePlayFunctionLibrary.h"

#include "MAttributeComponent.h"
#include "MPlayerController.h"
#include "MPlayerState.h"

bool UMGamePlayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount)
{
	UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(InstigatorActor, -DamageAmount);
	}
	return false;
}

bool UMGamePlayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if(ApplyDamage(InstigatorActor,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if(HitComp&&HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			HitComp->AddImpulseAtLocation(Direction * 200000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}

bool UMGamePlayFunctionLibrary::UpdatePlayerCredits(AMPlayerController* PlayerController, float Delta)
{
	AMPlayerState* State = PlayerController->GetPlayerState<AMPlayerState>();
	if(State)
	{
		if(Delta<0.0f&&State->Credits+Delta<0.0f)
		{
			return false;
		}
		else
		{
			State->Credits += Delta;
			PlayerController->OnCreditsChanged.Broadcast(State->Credits);
			return true;
		}
	}
	return false;
}
