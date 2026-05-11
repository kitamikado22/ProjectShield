// Fill out your copyright notice in the Description page of Project Settings.


#include "PSMainHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UPSMainHUDWidget::MarkLockOnPointerToTarget(const AActor* InTargetActor)
{
    if (not InTargetActor)
    {
        LockOnPointer->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    FVector WorldLocation = InTargetActor->GetActorLocation();
    FVector2D ScreenPosition;

    // 3Dワールド空間から、Widget用の座標に変換
    bool bProjected = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
        GetOwningPlayer(), WorldLocation, ScreenPosition, true);

    if (!bProjected)
    {
        LockOnPointer->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    LockOnPointer->SetVisibility(ESlateVisibility::Visible);

    if (UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LockOnPointer))
    {
        CanvasSlot->SetPosition(ScreenPosition);
    }

}