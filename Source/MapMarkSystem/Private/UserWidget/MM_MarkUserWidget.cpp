// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MM_MarkUserWidget.h"

void UMM_MarkUserWidget::ShowMark_Implementation()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMM_MarkUserWidget::HideMark_Implementation()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMM_MarkUserWidget::MarkEnterOrLeaveView(bool IsEnter)
{
	if (IsEnter)
	{
		if (!bMarkIsEnterView)
		{
			bMarkIsEnterView = true;
			MarkEnterView();
		}
	}
	else
	{
		if (bMarkIsEnterView)
		{
			bMarkIsEnterView = false;
			MarkLeaveView();
		}
	}
}

void UMM_MarkUserWidget::MarkEnterView_Implementation()
{
}

void UMM_MarkUserWidget::MarkLeaveView_Implementation()
{
}

void UMM_MarkUserWidget::UpdateMark_Implementation(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo NewMarkInfo)
{
	MarkInfo = NewMarkInfo;
}

void UMM_MarkUserWidget::SetDistance_Implementation(float Distance, const FText& DistanceText)
{
}
