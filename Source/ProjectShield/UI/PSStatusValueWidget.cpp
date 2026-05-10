// Fill out your copyright notice in the Description page of Project Settings.


#include "PSStatusValueWidget.h"
#include "Components/ProgressBar.h"

void UPSStatusValueWidget::UpdateMaxValue(float NewValue)
{
	CurrentMaxValue = NewValue;
	UpdateProgressBar();
}

void UPSStatusValueWidget::UpdateValue(float NewValue)
{
	CurrentValue = NewValue;
	UpdateProgressBar();
}

void UPSStatusValueWidget::UpdateProgressBar()
{
	if (ProgressBar && CurrentMaxValue > 0.0f)
	{
		ProgressBar->SetPercent(CurrentValue / CurrentMaxValue);
	}
}