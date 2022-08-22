// Fill out your copyright notice in the Description page of Project Settings.


#include "MMagicProjectile.h"

#include "MAttributeComponent.h"
#include "MGamePlayFunctionLibrary.h"
#include "Action/MActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMMagicProjectile::AMMagicProjectile():Damage(20.0f),bReflected(false)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //MovementComp->InitialSpeed = 4000.0f;
    SphereComp->SetSphereRadius(15.0f);
}

void AMMagicProjectile::DealDamage(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int Index, bool bArg, const FHitResult& HitResult)
{
    if(OtherActor&&OtherActor!=GetInstigator())
    {
	    if (!bReflected)
	    {
			UMActionComponent* ActionComp = Cast<UMActionComponent>(OtherActor->GetComponentByClass(UMActionComponent::StaticClass()));
		    if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		    {
			    MovementComp->Velocity = -MovementComp->Velocity;
			    SetInstigator(Cast<APawn>(OtherActor));
			    bReflected = true;
				return;
		    }
	    }
        if(UMGamePlayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, HitResult))
        {
            UE_LOG(LogTemp, Log, TEXT("%s damage %s"), *GetNameSafe(GetInstigator()), *GetNameSafe(OtherActor));
            Explode();
        }
    }
}

void AMMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
    SphereComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMMagicProjectile::DealDamage);
}

