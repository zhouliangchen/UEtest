// Fill out your copyright notice in the Description page of Project Settings.


#include "MMagicProjectile.h"

#include "MAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AMMagicProjectile::DealDamage(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int Index, bool bArg, const FHitResult& HitResult)
{
    if(OtherActor&&OtherActor!=GetInstigator())
    {
        UMAttributeComponent* AttributeComp = Cast<UMAttributeComponent>(OtherActor->GetComponentByClass(UMAttributeComponent::StaticClass()));
        if(AttributeComp)
        {
            AttributeComp->ApplyHealthChange(-20.0f);
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, GetNameSafe(this) + " damage " + GetNameSafe(OtherActor));
            Destroy();
        }
    }
}

// Sets default values
AMMagicProjectile::AMMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //MovementComp->InitialSpeed = 4000.0f;
    SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMMagicProjectile::DealDamage);
}