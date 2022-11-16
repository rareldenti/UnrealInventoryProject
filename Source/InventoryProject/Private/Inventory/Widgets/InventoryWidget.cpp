// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/InventoryWidget.h"
#include "Components/ScrollBox.h"
#include "Inventory/Widgets/OutsideItemWidget.h"
#include "Inventory/ItemActor/Item.h"
#include "Inventory/InventoryComponent.h"
#include "Components/CanvasPanel.h"
#include "Inventory/Widgets/InsideItemWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryProject/InventoryProjectCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Inventory/InventoryComponent.h"
#include "Components/Button.h"
#include "Inventory/Widgets/MenuBarWidget.h"

void UInventoryWidget::Init()
{
	ScrollBox = Cast<UScrollBox>(this->GetWidgetFromName("OutsideItemScrollBox"));
	CanvasPanle = Cast<UCanvasPanel>(this->GetWidgetFromName("InsideItemCanvasPanel"));
	DropButton = Cast<UButton>(this->GetWidgetFromName("DropButton"));
	MenuBarWidget = Cast<UMenuBarWidget>(this->GetWidgetFromName("MenuBar"));
	InventoryComponent = Cast<AInventoryProjectCharacter>(GetOwningPlayer()->GetCharacter())->GetInventoryComponent();

	DropButton->SetVisibility(ESlateVisibility::Hidden);
	MenuBarWidget->SetVisibility(ESlateVisibility::Hidden);

	MenuBarWidget->InitWidget(InventoryComponent,this);
}

void UInventoryWidget::CreateNewOutsideItemWidget(FOutsideItem& OutsideItem)
{
	auto OutsideWidget = Cast<UOutsideItemWidget>(CreateWidget(ScrollBox, OutsideItemWidgetClass));
	OutsideWidget->Init();
	OutsideWidget->SetWidgetStyel(OutsideItem.ItemDetails);

	OutsideItem.Widget = OutsideWidget;

	ScrollBox->AddChild(OutsideWidget);
}

void UInventoryWidget::ClearWidgetInOutsideBox()
{
	auto Widgets = ScrollBox->GetAllChildren();

	for (size_t i = 0; i < Widgets.Num(); i++)
	{
		Widgets[i]->ConditionalBeginDestroy();
		Widgets.RemoveAt(i);
	}
	ScrollBox->ClearChildren();

	return;
}

void UInventoryWidget::InitInsideItemBox(FVector2D InventoryMaxStorage, TArray<FInsideItem>& InsideItemBox)
{
	FVector2D SlotPosition = InsideBoxBeginPoint;

	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		auto InsideWidget = Cast<UInsideItemWidget>(CreateWidget(CanvasPanle, UInsideItemWidgetClass));
		InsideWidget->SetInventoryWidget(this);

		InsideItemBox[i].Widget = InsideWidget;
		CanvasPanle->AddChild(InsideWidget);
		Cast<UCanvasPanelSlot>(InsideWidget->Slot)->SetPosition(SlotPosition);

		if ((SlotPosition.X - InsideBoxBeginPoint.X) + InsideItemBoxSize.X >= InventoryMaxStorage.X * InsideItemBoxSize.X)
		{
			SlotPosition.Y += InsideItemBoxSize.Y;
			SlotPosition.X = InsideBoxBeginPoint.X;
		}
		else
		{
			SlotPosition.X += InsideItemBoxSize.X;
		}
	}
}

void UInventoryWidget::RefreshInsideItemBox(TArray<FInsideItem>& InsideItemBox)
{
	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		InsideItemBox[i].Widget->Init();
		InsideItemBox[i].Widget->SetWidgetStyle(InsideItemBox[i].ItemDetails);
	}
}

void UInventoryWidget::SetTargetWidget(UInsideItemWidget* Widget)
{
	TargetWidget = Widget;
	bIsHadWidgetFollowMouse = true;
}

void UInventoryWidget::ResetTargetWidget()
{
	if (!TargetWidget)
		return;

	TargetWidget->SetVisibility(ESlateVisibility::Visible);
	Cast<UCanvasPanelSlot>(TargetWidget->Slot)->SetPosition(TargetWidget->GetSelfPosition());
	Cast<UCanvasPanelSlot>(TargetWidget->Slot)->SetZOrder(0);

	TargetWidget = nullptr;
	bIsHadWidgetFollowMouse = false;

	DropButton->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::TargetWidgetFollowMouse()
{
	if (!TargetWidget)
		return;

	auto NowMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	Cast<UCanvasPanelSlot>(TargetWidget->Slot)->SetPosition(NowMousePosition - TargetWidget->GetWhenPressedMousePosition() + TargetWidget->GetSelfPosition());

	DropButton->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryWidget::SwitchTwoWidget(UInsideItemWidget* Widget)
{
	if (TargetWidget && Widget)
	{
		InventoryComponent->SwitchTwoItemDetail(TargetWidget, Widget);
		ResetTargetWidget();
	}
}

void UInventoryWidget::DropThisItem()
{
	InventoryComponent->DropThisItem(TargetWidget, InventoryComponent->GetItemDetails(TargetWidget).Num);
	ResetTargetWidget();
}

void UInventoryWidget::SetHoveredWidget(UInsideItemWidget* Widget)
{
	HoveredWidget = Widget;
	MenuBarWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryWidget::ShowMenuBar()
{
	if (!HoveredWidget)
		return;

	if (MenuBarWidget->IsVisible())
	{
		MenuBarWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	auto NowMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	MenuBarWidget->SetPositionInViewport(NowMousePosition);
	Cast<UCanvasPanelSlot>(MenuBarWidget->Slot)->SetPosition(NowMousePosition - FVector2D(960.960938, 540.540527));
	MenuBarWidget->SetVisibility(ESlateVisibility::Visible);
	MenuBarWidget->RefreshItemDetails(HoveredWidget);
}

bool UInventoryWidget::IsThisItemIDIlligel(UInsideItemWidget* Widget)
{
	return InventoryComponent->GetItemDetails(Widget).ID != -1;
}
