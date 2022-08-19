// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction_Projectile.h"

#include "MCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UMAction_Projectile::UMAction_Projectile():AttackDelay(0.2f),HandSocket("Muzzle_01")
{
}

void UMAction_Projectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AnimAttack);
		UGameplayStatics::SpawnEmitterAttached(SpellVFX, Character->GetMesh(), HandSocket, FVector::ZeroVector,FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMAction_Projectile::SpawnProjectile, Character);
		GetWorld()->GetTimerManager().SetTimer(AttackTimeHandle, Delegate, AttackDelay, false);
	}
}
	
void UMAction_Projectile::SpawnProjectile(ACharacter* Instigator)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector CameraLocation = Instigator->GetPawnViewLocation();
		FVector End = CameraLocation + Instigator->GetControlRotation().Vector() * 10000;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, End, ECC_WorldDynamic, Params);
		FVector TargetPoint = bHit ? HitResult.ImpactPoint : End;
		FVector RhandLocation = Instigator->GetMesh()->GetSocketLocation(HandSocket);
		FVector SubVector = TargetPoint - RhandLocation;
		//SubVector.Rotation()
		//FRotator AimRotator(SubVector.X, SubVector.Y, SubVector.Z);
		//FTransform SpawnTM(SubVector.Rotation(), RhandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Instigator;
		GetWorld()->SpawnActor<AMProjectileBase>(ProjectileClass, RhandLocation, SubVector.Rotation(), SpawnParams);

		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, GetNameSafe(Projectile) + TEXT("“—∑¢…‰"));
	}
	StopAction(Instigator);
}
