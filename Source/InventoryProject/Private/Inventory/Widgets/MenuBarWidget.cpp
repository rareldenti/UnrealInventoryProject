// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/MenuBarWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Widgets/InsideItemWidget.h"
#include "InventoryProject/Public/Inventory/ItemActor/Item.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Inventory/Widgets/InventoryWidget.h"

void UMenuBarWidget::InitWidget(UInventoryComponent* Component, UInventoryWidget* InventoryWidgetComponent)
{
	InventoryComponent = Component;
	InventoryWidget = InventoryWidgetComponent;
	ItemName = Cast<UTextBlock>(GetWidgetFromName("ItemName"));
	NumText = Cast<UEditableText>(GetWidgetFromName("Num"));
}

void UMenuBarWidget::RefreshItemDetails(UInsideItemWidget* Widget)
{
	TargetWidget = Widget;
	auto Details = InventoryComponent->GetItemDetails(Widget);

	if (Details.ID == -1)
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	ItemName->SetText(Details.ItemName);
}

int UMenuBarWidget::GetNum()
{
	auto nums = NumText->GetText();
	auto Number = FCString::Atoi(*nums.ToString());
	return Number;
}

void UMenuBarWidget::UseItem()
{
	auto Details = InventoryComponent->GetItemDetails(TargetWidget);

	Details.Num = FMath::Clamp(Details.Num - GetNum(), 0, Details.Num);
	if (Details.Num == 0)
	{
		Details.ID = -1;
		this->SetVisibility(ESlateVisibility::Hidden);
	}

	InventoryComponent->SetItemDetails(TargetWidget, Details);
	InventoryWidget->RefreshInsideItemBox(InventoryComponent->GetInsideItemBox());

	return;
}

void UMenuBarWidget::DropItem()
{
	InventoryComponent->DropThisItem(TargetWidget, GetNum());
	this->SetVisibility(ESlateVisibility::Hidden);

	return;
}
