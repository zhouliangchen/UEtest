// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MAttachedWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class UETEST_API UMAttachedWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachTo;
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector Offset;
public:
	UMAttachedWidget();
	bool SetAttachedActor(AActor* Actor);
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	
};
