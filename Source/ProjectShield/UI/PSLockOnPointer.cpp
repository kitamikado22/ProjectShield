// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PSLockOnPointer.h"


void UPSLockOnPointer::NativeConstruct()
{
    Super::NativeConstruct();

    PlayAnimation(PulseAnimation, 0.f, 0);
}

void UPSLockOnPointer::Hide()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UPSLockOnPointer::Show()
{
    this->SetVisibility(ESlateVisibility::Visible);
}