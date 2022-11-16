// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UOutsideItemWidget;
class UInsideItemWidget;
class UScrollBox;
class UCanvasPanel;
class UButton;
class UMenuBarWidget;
struct FOutsideItem;
struct FInsideItem;
struct FItemDetails;

UCLASS()
class INVENTORYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

	void CreateNewOutsideItemWidget(FOutsideItem& OutsideItem);
	void ClearWidgetInOutsideBox();

	void InitInsideItemBox(FVector2D InventoryMaxStorage, TArray<FInsideItem>& InsideItemBox);
	void RefreshInsideItemBox(TArray<FInsideItem>& InsideItemBox);

	void SetTargetWidget(UInsideItemWidget* Widget);
	void TargetWidgetFollowMouse();
	bool GetIsHadWidgetFollowMouse() { return bIsHadWidgetFollowMouse; };
	UFUNCTION(BlueprintCallable)
	void ResetTargetWidget();

	void SwitchTwoWidget(UInsideItemWidget* Widget);
	UFUNCTION(BlueprintCallable)
	void DropThisItem();

	void SetHoveredWidget(UInsideItemWidget* Widget);
	UFUNCTION(BlueprintCallable)
	void ShowMenuBar();

	bool IsThisItemIDIlligel(UInsideItemWidget* Widget);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UOutsideItemWidget> OutsideItemWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInsideItemWidget> UInsideItemWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D InsideItemBoxSize = FVector2D(100, 100);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D InsideBoxBeginPoint = FVector2D(10, 10);

private:
	UScrollBox* ScrollBox;
	UButton* DropButton;
	UCanvasPanel* CanvasPanle;
	UMenuBarWidget* MenuBarWidget;
	
	bool bIsHadWidgetFollowMouse = false;
	UInsideItemWidget* TargetWidget = nullptr;
	UInsideItemWidget* HoveredWidget = nullptr;

	UInventoryComponent* InventoryComponent;
};
