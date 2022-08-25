// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MInteractComponent.generated.h"


class UMAttachedWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UETEST_API UMInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	void PrimaryInteract();

protected:
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMAttachedWidget> InteractWidgetClass;

	UPROPERTY()
	UMAttachedWidget* InteractWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	int32 InteractDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	void InteractDetection();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Sets default values for this component's properties
	UMInteractComponent();

		
};
