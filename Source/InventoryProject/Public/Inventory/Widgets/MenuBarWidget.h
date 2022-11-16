// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBarWidget.generated.h"

class UInventoryComponent;
class UTextBlock;
class UInsideItemWidget;
class UEditableText;
class UInventoryWidget;
struct FItemDetails;

UCLASS()
class INVENTORYPROJECT_API UMenuBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitWidget(UInventoryComponent* Component, UInventoryWidget* InventoryWidget);
	void RefreshItemDetails(UInsideItemWidget* Widget);

	int GetNum();
	UFUNCTION(BlueprintCallable)
	void UseItem();
	UFUNCTION(BlueprintCallable)
	void DropItem();

private:
	UTextBlock* ItemName;
	UEditableText* NumText;
	UInventoryComponent* InventoryComponent;
	UInventoryWidget* InventoryWidget;

	UInsideItemWidget* TargetWidget;
};
