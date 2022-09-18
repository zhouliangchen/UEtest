// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/MAnimInstance.h"

#include "Action/MActionComponent.h"

void UMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ActionComp = UMActionComponent::GetActionComp(GetOwningActor());
}

void UMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	static FGameplayTag SprintTag = FGameplayTag::RequestGameplayTag("Action.Sprinting");
	if(ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
		bIsSprinting = ActionComp->ActiveGameplayTags.HasTag(SprintTag);
	}
}
