// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/MAction_Projectile.h"

#include "MAttributeComponent.h"
#include "MProjectileBase.h"
#include "MCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UMAction_Projectile::UMAction_Projectile():EnergyCost(0.0f),AttackDelay(0.2f),HandSocket("Muzzle_01")
{
}

void UMAction_Projectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(EnergyCost)
	{
		UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(Character);
		// 这里默认前面已做过CanStart检查
		AttributeComp->ApplyEnergyChange(Character, -EnergyCost);
	}
	if (Character)
	{
		Character->PlayAnimMontage(AnimAttack);
		UGameplayStatics::SpawnEmitterAttached(SpellVFX, Character->GetMesh(), HandSocket, FVector::ZeroVector,FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UMAction_Projectile::SpawnProjectile, Character);
		GetWorld()->GetTimerManager().SetTimer(AttackTimeHandle, Delegate, AttackDelay, false);
	}
}

bool UMAction_Projectile::CanStart_Implementation(AActor* Instigator)
{
	if(Super::CanStart_Implementation(Instigator))
	{
		if (EnergyCost == 0.0f)return true;
		ACharacter* Character = Cast<ACharacter>(Instigator);
		UMAttributeComponent* AttributeComp = UMAttributeComponent::GetAttributeComp(Character);
		if(AttributeComp)
		{
			if (AttributeComp->IsEnergyEnough(EnergyCost))return true;
			else
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Purple, TEXT("Energy不足！"));
			}
		}
	}
	return false;
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

		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, GetNameSafe(Projectile) + TEXT("已发射"));
	}
	StopAction(Instigator, false);
}
