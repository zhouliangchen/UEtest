// Fill out your copyright notice in the Description page of Project Settings.


#include "MInteractComponent.h"
#include "MGameplayInterface.h"
#include "Camera/CameraComponent.h"
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
	AActor* MyActor = GetOwner();
	UCameraComponent* CameraComp = Cast<UCameraComponent>(MyActor->GetComponentByClass(UCameraComponent::StaticClass()));
	if(ensure(CameraComp))
	{
		FVector Start = CameraComp->GetComponentLocation();
		FRotator Rot = CameraComp->GetComponentRotation();
		FVector End = Start + Rot.Vector() * 500;
		FHitResult HitResult;
		constexpr float Radius = 30.0f;
		FCollisionShape Shape;
		Shape.SetSphere(Radius);

		bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ECC_WorldDynamic, Shape);
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		if (bHit)
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
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("非法使用交互组件！"));
	}
	//auto& CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//FVector Start = CamManager->GetCameraLocation();
	//FRotator Rot = CamManager->GetCameraRotation();
	//MyActor->GetActorEyesViewPoint(Start, Rot);

	
	//FCollisionObjectQueryParams ObjectParams(ECC_WorldDynamic);
	//ObjectParams.AddObjectTypesToQuery()
	//
	// bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ECC_WorldDynamic);
	//TArray<FHitResult> HitResults;
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

