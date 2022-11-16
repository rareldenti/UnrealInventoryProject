// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/InsideItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Inventory/ItemActor/Item.h"
#include "Inventory/Widgets/InventoryWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UInsideItemWidget::Init()
{
	ItemIcon = Cast<UImage>(this->GetWidgetFromName("ItemIcon"));
	ItemNum = Cast<UTextBlock>(this->GetWidgetFromName("ItemNum"));
	ItemButton = Cast<UButton>(this->GetWidgetFromName("ItemButton"));
}

void UInsideItemWidget::SetWidgetStyle(FItemDetails& Details)
{
	if (Details.ID == -1)
	{
		this->ItemIcon->SetBrushResourceObject(EmptyImg);
		this->ItemNum->SetText(FText::FromString(""));
		this->ItemButton->SetToolTipText(FText::FromString(""));
		return;
	}

	this->ItemIcon->SetBrushResourceObject(Details.ItemIcon);
	this->ItemNum->SetText(FText::FromString(FString::FormatAsNumber(Details.Num)));
	this->ItemButton->SetToolTipText(Details.ItemName);
}

void UInsideItemWidget::BeClicked()
{
	if (!InventoryWidget->IsThisItemIDIlligel(this) && (InventoryWidget->GetIsHadWidgetFollowMouse() == 0))
		return;

	if (InventoryWidget->GetIsHadWidgetFollowMouse())
	{
		InventoryWidget->SwitchTwoWidget(this);
	}
	else
	{
		SelfPosition = Cast<UCanvasPanelSlot>(this->Slot)->GetPosition();
		WhenPressedMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		InventoryWidget->SetTargetWidget(this);

		Cast<UCanvasPanelSlot>(this->Slot)->SetZOrder(1);
		this->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UInsideItemWidget::BeHovered()
{
	InventoryWidget->SetHoveredWidget(this);
}