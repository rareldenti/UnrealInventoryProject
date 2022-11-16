// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Widgets/OutsideItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/ItemActor/Item.h"
#include "InventoryProject/InventoryProjectCharacter.h"
#include "Inventory/InventoryComponent.h"

void UOutsideItemWidget::Init()
{
	this->ItemIcon = Cast<UImage>(this->GetWidgetFromName("ItemIcon"));
	this->ItemName = Cast<UTextBlock>(this->GetWidgetFromName("ItemName"));
	this->ItemDiscription = Cast<UTextBlock>(this->GetWidgetFromName("ItemDiscription"));
	this->ItemNum = Cast<UTextBlock>(this->GetWidgetFromName("ItemNum"));
}

void UOutsideItemWidget::SetWidgetStyel(FItemDetails& Details)
{
	this->ItemIcon->SetBrushResourceObject(Details.ItemIcon);
	this->ItemName->SetText(Details.ItemName);
	this->ItemDiscription->SetText(Details.ItemDiscription);
	this->ItemNum->SetText(FText::FromString(FString::FormatAsNumber(Details.Num)));
}

void UOutsideItemWidget::WhenClicked()
{
	Cast<AInventoryProjectCharacter>(GetOwningPlayer()->GetCharacter())->GetInventoryComponent()->PickUpByWidget(this);
}
