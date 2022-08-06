// Fill out your copyright notice in the Description page of Project Settings.


#include "MProjectileBase.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMProjectileBase::AMProjectileBase()
{

	RootComponent = SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionProfileName("Projectile");

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
	ParticleComp->SetupAttachment(SphereComp);
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 4000.0f;
	
}

void AMProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddUniqueDynamic(this, &AMProjectileBase::ProjectileHit);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AMProjectileBase::ProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Blue, GetNameSafe(this) + " hit " + GetNameSafe(OtherActor));

	Explode();
}

void AMProjectileBase::Explode_Implementation()
{
	if(ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
		Destroy();
	}
}

