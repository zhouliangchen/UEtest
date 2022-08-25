// Fill out your copyright notice in the Description page of Project Settings.


#include "MInteractComponent.h"
#include "MGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameSession.h"
#include "UI/MAttachedWidget.h"

TAutoConsoleVariable<bool> CVarDebugDraw(TEXT("mt.DebugDraw"), false, TEXT("控制显示Debug图形信息"), ECVF_Cheat);
// Sets default values for this component's properties
UMInteractComponent::UMInteractComponent():InteractDistance(500), TraceSphereRadius(30.0f), CollisionChannel(ECC_WorldDynamic)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

// Called every frame
void UMInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	InteractDetection();
	// ...
}


void UMInteractComponent::InteractDetection()
{
	AActor* MyActor = GetOwner();
	UCameraComponent* CameraComp = Cast<UCameraComponent>(MyActor->GetComponentByClass(UCameraComponent::StaticClass()));
	if (ensure(CameraComp))
	{
		FVector Start = CameraComp->GetComponentLocation();
		FRotator Rot = CameraComp->GetComponentRotation();
		FVector End = Start + Rot.Vector() * InteractDistance;
		FHitResult HitResult;
		FCollisionShape Shape;
		Shape.SetSphere(TraceSphereRadius);
		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(CollisionChannel);
		bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, Params, Shape);
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;

		const bool bDebugDraw = CVarDebugDraw.GetValueOnGameThread();
		FocusedActor = nullptr;
		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, "HitActor: " + GetNameSafe(HitActor));
			if (HitActor->Implements<UMGameplayInterface>())
			{
				FocusedActor = HitActor;
			}
			if (bDebugDraw)
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceSphereRadius, 16, DebugColor, false, 0.0f, 0,2.0f);
		}
		if (FocusedActor)
		{
			if (!InteractWidgetInstance && InteractWidgetClass)
			{
				InteractWidgetInstance = CreateWidget<UMAttachedWidget>(GetWorld(), InteractWidgetClass);
			}
			if (InteractWidgetInstance)
			{
				InteractWidgetInstance->AttachTo = FocusedActor;
				if (!InteractWidgetInstance->IsInViewport())
				{
					InteractWidgetInstance->AddToViewport();
				}
			}
		}
		else
		{
			if(InteractWidgetInstance&&InteractWidgetInstance->IsInViewport())
			{
				InteractWidgetInstance->RemoveFromParent();
			}
		}
		if (bDebugDraw)
			DrawDebugLine(GetWorld(), Start, bHit ? HitResult.ImpactPoint : End, DebugColor, false, 0.0f, 0,2.0f);
	}
}

void UMInteractComponent::PrimaryInteract()
{
	if(FocusedActor)
	{
		APawn* MyPawn = Cast<APawn>(GetOwner());
		IMGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("当前无可交互物体！"));
	}
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

// Called when the game starts
void UMInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


