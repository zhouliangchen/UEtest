// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class UMAttachedWidget;
class USpringArmComponent;
class UCameraComponent;
class UMInteractComponent;
class AMProjectileBase;
class UMAttributeComponent;
class UMActionComponent;

UCLASS()
class UETEST_API AMCharacter : public ACharacter
{
	GENERATED_BODY()
protected:

	//为了更好地实现HitFlash判定逻辑
	//更新MAttributeComponent后已不再需要，可考虑弃用，目前仅作debug用途
	//bool bIsAlive;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName HitTimeParam;
public:
	// Sets default values for this character's properties
	AMCharacter();
	virtual FVector GetPawnViewLocation() const override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Exec)
	bool HealSelf(float HealAmount = 0.0f);

	//在ListenServer上，Spawn Player时，触发beginPlay，而后再被Possessed，因此需要额外的函数辅助判断是否为本地控制角色
	void OnServerPlayerSpawned();
	
protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UMInteractComponent* InteractComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMAttachedWidget> HealthBarWidgetClass;

	UPROPERTY()
	UMAttachedWidget* HealthBarWidgetInstance;
	void MoveForward(float X);
	void MoveRight(float X);
	void PrimaryAttack();
	void BlackholeAttack();
	void Teleport();
	void SprintStart();
	void SprintStop();
	void Parry();
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void OnHealthReplicated(UMAttributeComponent* OwningComp, float NewHealth);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
};
