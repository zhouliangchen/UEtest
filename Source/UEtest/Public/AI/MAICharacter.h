// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MAICharacter.generated.h"

class UMActionComponent;
class UMAttachedWidget;
class UPawnSensingComponent;
class UMAttributeComponent;

UCLASS()
class UETEST_API AMAICharacter : public ACharacter
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocket;

// Sets default values for this character's properties
	AMAICharacter();
	//static int GetBotNum();
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMAttributeComponent* AttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FName HitTimeParam;//���ʲ���������������ʱ�䣬����ֵˢ����0���Ӷ�����HitFlash

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsLowHealth;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float HealthThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<UMAttachedWidget> HealthBarWidgetClass;

	UPROPERTY()
	UMAttachedWidget* HealthBarWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<UMAttachedWidget> SpotWidgetClass;

	UPROPERTY()
	UMAttachedWidget* SpotWidgetInstance;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpot();

	bool SetTargetActor(AActor* TargetActor);
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMAttributeComponent* OwningComp, float NewHealth, float Delta);
	UFUNCTION()
	void OnHealthReplicated(UMAttributeComponent* OwningComp, float NewHealth);
	virtual void PostInitializeComponents()override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	//ʹ�þ�̬��Ա�������ڶ�����Ϸ�б����쳣�������ж������server��client��˫��ͬ����Ŀǰ��Ϊʹ��GameMode
	//inline static int ExistBotNum = 0;
};
