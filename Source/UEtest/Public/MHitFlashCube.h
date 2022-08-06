// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MHitFlashCube.generated.h"

class UMAttributeComponent;
UCLASS()
class UETEST_API AMHitFlashCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMHitFlashCube();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMAttributeComponent* AttributeComp;

	UFUNCTION()
	void HitFlash(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	virtual	void PostInitializeComponents()override;


public:	


};
