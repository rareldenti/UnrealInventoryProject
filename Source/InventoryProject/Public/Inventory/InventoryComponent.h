// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemActor/Item.h"
#include "InventoryComponent.generated.h"

class UOutsideItemWidget;
class UInsideItemWidget;
class UInventoryWidget;
class UUserWidget;

struct FOutsideItem
{
	FItemDetails ItemDetails;
	UOutsideItemWidget* Widget = nullptr;
};

struct FInsideItem
{
	FItemDetails ItemDetails;
	UInsideItemWidget* Widget = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();

	void ToggleInventoryWidget();

	void GetAllItems();
	bool CheckIfAlreadyInArray(AItem* Item);
	void AddItemInDistence(AItem* Item);
	void DeleteItemNotInDistence(AItem* Item);
	int32 FindIndexOfInDistenceArray(AItem* Item);

	void ConvertItemToOutsideItem();
	bool LeachSameIDItem(FItemDetails Details);
	void UpdataOutsideBox();

	void PickUpByKey();
	void PickUpByWidget(UOutsideItemWidget* Widget);
	AItem* FindClosetItem(int32 ItemID);
	void AddToInsideBox(FItemDetails Details);
	int32 FindIndexCouldSaveItem();
	bool CheckAlreayInInventory(FItemDetails Details);

	void SwitchTwoItemDetail(UInsideItemWidget* OriginalWidget, UInsideItemWidget* TargetWidget);
	bool DropThisItem(UInsideItemWidget* Widget,int32 DropNum);
	void SpawnItem(FItemDetails Details);

	FItemDetails GetItemDetails(UInsideItemWidget* Widget);
	void SetItemDetails(UInsideItemWidget* Widget, FItemDetails NewDetails);

	TArray<FInsideItem>& GetInsideItemBox() { return InsideItemBox; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	double PickUpDistence = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D InventoryMaxStorage = FVector2D(5,5);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemFloatWIdget;

private:
	TArray<AItem*> AllInDistenceItems;
	TArray<FOutsideItem> OutsideItemBox;
	TArray<FInsideItem> InsideItemBox;

	UInventoryWidget* InventoryWidget;
};
