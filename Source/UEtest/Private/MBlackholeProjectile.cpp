// Fill out your copyright notice in the Description page of Project Settings.


#include "MBlackholeProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMBlackholeProjectile::AMBlackholeProjectile()
{
	MovementComp->InitialSpeed= 2000.0f;
	SphereComp->SetSphereRadius(55.0f);
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(SphereComp);
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->Falloff = RIF_Linear;
	RadialForceComp->ForceStrength = 2000000.0f;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AMBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();

}
