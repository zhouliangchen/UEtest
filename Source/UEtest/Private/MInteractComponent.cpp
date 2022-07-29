// Fill out your copyright notice in the Description page of Project Settings.


#include "MInteractComponent.h"
#include "MGameplayInterface.h"
#include "GameFramework/GameSession.h"

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
	auto& CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector Start = CamManager->GetCameraLocation();
	FRotator Rot = CamManager->GetCameraRotation();
	AActor* MyActor = GetOwner();
	//MyActor->GetActorEyesViewPoint(Start, Rot);

	FVector End = Start + Rot.Vector() * 500;
	
	//FCollisionObjectQueryParams ObjectParams(ECC_WorldDynamic);
	//ObjectParams.AddObjectTypesToQuery()
	//
	FHitResult HitResult;
	// bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ECC_WorldDynamic);
	constexpr float Radius = 30.0f;
	//TArray<FHitResult> HitResults;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ECC_WorldDynamic, Shape);
	const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
	if(bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, "HitActor: " + GetNameSafe(HitActor));
		if (HitActor->Implements<UMGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyActor);
			IMGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 16, DebugColor, false, 2.0, 0, 2.0);
	}
	DrawDebugLine(GetWorld(), Start, bHit ? HitResult.ImpactPoint : End, DebugColor, false, 2.0, 0, 2.0);
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

