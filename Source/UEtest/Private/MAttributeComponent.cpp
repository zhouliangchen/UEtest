// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeComponent.h"


// Sets default values for this component's properties
UMAttributeComponent::UMAttributeComponent():Health(100.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// // Called every frame
// void UMAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	GEngine->AddOnScreenDebugMessage(1, 1.0, FColor::Yellow, FString::SanitizeFloat(Health));
// }

bool UMAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true;
}

