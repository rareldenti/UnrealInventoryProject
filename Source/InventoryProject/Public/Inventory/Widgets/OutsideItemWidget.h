// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutsideItemWidget.generated.h"

class UImage;
class UTextBlock;
struct FItemDetails;

UCLASS()
class INVENTORYPROJECT_API UOutsideItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init();

	void SetWidgetStyel(FItemDetails& Details);

	UFUNCTION(BlueprintCallable)
	void WhenClicked();

private:
	UImage* ItemIcon;
	UTextBlock* ItemName;
	UTextBlock* ItemDiscription;
	UTextBlock* ItemNum;
};
