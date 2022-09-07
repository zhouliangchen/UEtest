// Fill out your copyright notice in the Description page of Project Settings.


#include "MProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


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

	//SetReplicates(true);
	bReplicates = true;
	//SetReplicatingMovement(true);
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
	UE_LOG(LogTemp, Log, TEXT("%s hit %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));
	MulticastExplode();
}

void AMProjectileBase::Explode_Implementation()
{
	if(!bActive.test_and_set())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(this, Shake, GetActorLocation(), 200.0f, 2000.0f);
		SetLifeSpan(3.0f);
		MovementComp->StopMovementImmediately();
		AudioComp->Stop();
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
}
void AMProjectileBase::MulticastExplode_Implementation()
{
	Explode();
}
