// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "MItemChest.generated.h"

UCLASS()
class UETEST_API AMItemChest : public AActor, public IMGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMItemChest();

	float RotationPitch;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	//����generated headfile������
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	//�ڶ�����ΪҪ���Ƶ����Խ���Replicated��ʶ
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing="OnRep_LidOpened")
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ChestMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
