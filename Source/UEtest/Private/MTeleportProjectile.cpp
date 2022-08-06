// Fill out your copyright notice in the Description page of Project Settings.

#include "MTeleportProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMTeleportProjectile::AMTeleportProjectile():MoveTime(0.8f),TeleportTime(0.2f)
{
	MovementComp->InitialSpeed = 2000.0f;
}


void AMTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TeleportTimer, this, &AMTeleportProjectile::Explode, MoveTime);
}
//复用了定时器句柄，稍微注意点
void AMTeleportProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TeleportTimer);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	ParticleComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(TeleportTimer, this, &AMTeleportProjectile::Teleport, TeleportTime);
}
void AMTeleportProjectile::Teleport()
{
	APawn* instigator = GetInstigator();
	if(instigator)
	{
		instigator->TeleportTo(GetActorLocation(), instigator->GetActorRotation());
	}
	//可能角色在被传送前阵亡
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, TEXT("找不到传送对象"));
	}
	Destroy();
}
