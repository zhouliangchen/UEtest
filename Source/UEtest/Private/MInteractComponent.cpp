// Fill out your copyright notice in the Description page of Project Settings.


#include "MInteractComponent.h"

#include "MGameplayInterface.h"

// Sets default values for this component's properties
UMInteractComponent::UMInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void UMInteractComponent::PrimaryInteract()
{
	FVector Start;
	FRotator Rot;
	AActor* MyActor = GetOwner();
	MyActor->GetActorEyesViewPoint(Start, Rot);
	FVector End = Start + Rot.Vector() * 1000;
	FHitResult HitResult;
	//FCollisionObjectQueryParams ObjectParams(ECC_WorldDynamic);
	//ObjectParams.AddObjectTypesToQuery()
	GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ECC_WorldDynamic);
	if(AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<UMGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyActor);
			IMGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}
	
}


// Called when the game starts
void UMInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

