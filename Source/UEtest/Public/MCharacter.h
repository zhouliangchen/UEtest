// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMInteractComponent;
class AMProjectileBase;
class UMAttributeComponent;

UCLASS()
class UETEST_API AMCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AMProjectileBase> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AMProjectileBase> BlackholeClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AMProjectileBase> TeleportClass;


	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AnimAttack;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* SpellVFX;

	FTimerHandle PrimaryAttackTimeHandle;
	FTimerHandle BlackholeTimeHandle;
	FTimerHandle TeleportTimeHandle;

	//为了更好地实现HitFlash判定逻辑
	//更新MAttributeComponent后已不再需要，可考虑弃用，目前仅作debug用途
	bool bIsAlive;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName HitTimeParam;
public:
	// Sets default values for this character's properties
	AMCharacter();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	bool HealSelf(float HealAmount = 0.0f);
protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UMInteractComponent* InteractComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMAttributeComponent* AttributeComp;

	void MoveForward(float X);
	void MoveRight(float X);
	void PrimaryAttack();
	void PrimaryAttack_delay();
	void BlackholeAttack();
	void BlackholeAttack_delay();
	void Teleport();
	void Teleport_delay();
	void SpawnProjectile(TSubclassOf<AMProjectileBase> Projectile);
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	virtual void PostInitializeComponents() override;

};
