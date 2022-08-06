// Fill out your copyright notice in the Description page of Project Settings.


#include "MHitFlashCube.h"
#include "MAttributeComponent.h"

// Sets default values
AMHitFlashCube::AMHitFlashCube()
{
	RootComponent = MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	AttributeComp = CreateDefaultSubobject<UMAttributeComponent>("AttributeComp");
}


void AMHitFlashCube::HitFlash(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta<0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeOnHit", GetWorld()->TimeSeconds);
	}
}

void AMHitFlashCube::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddUniqueDynamic(this, &AMHitFlashCube::HitFlash);
}

