// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MAttachedWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

UMAttachedWidget::UMAttachedWidget():UUserWidget(FObjectInitializer()),Offset(0)
{

}

bool UMAttachedWidget::SetAttachedActorIfNot(AActor* Actor)
{
	if (AttachTo)return false;
	AttachTo = Actor;
	return true;
}

void UMAttachedWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!IsValid(AttachTo))
	{
		RemoveFromParent();
		return;
	}
	FVector2D ScreenPosition;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachTo->GetActorLocation() + Offset, ScreenPosition, true))
	{
		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition / UWidgetLayoutLibrary::GetViewportScale(this));
		}
	}
	

}
