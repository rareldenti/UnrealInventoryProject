// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/ItemActor/Item.h"
#include "InventoryProject/InventoryProjectCharacter.h"
#include "Components/WidgetComponent.h"
#include "Inventory/Widgets/InventoryWidget.h"
#include "Inventory/Widgets/OutsideItemWidget.h"
#include "Components/WidgetComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetAllItems();
	InventoryWidget->TargetWidgetFollowMouse();
}

void UInventoryComponent::Init()
{
	InventoryWidget = Cast<UInventoryWidget>(Cast<AInventoryProjectCharacter>(GetOwner())->GetWidgetComponent()->GetWidget());
	InventoryWidget->Init();

	FItemDetails NewDetails;
	FInsideItem NewInsideItem;
	NewInsideItem.ItemDetails = NewDetails;

	for (size_t i = 0; i < InventoryMaxStorage.X * InventoryMaxStorage.Y; i++)
	{
		InsideItemBox.Add(NewInsideItem);
	}

	InventoryWidget->InitInsideItemBox(InventoryMaxStorage, InsideItemBox);
}

void UInventoryComponent::ToggleInventoryWidget()
{
	auto WidgetComponent = Cast<AInventoryProjectCharacter>(GetOwner())->GetWidgetComponent();
	if (WidgetComponent->IsVisible())
	{
		WidgetComponent->SetVisibility(false);
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())->GetController())->SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())->GetController())->SetShowMouseCursor(false);
	}
	else
	{
		WidgetComponent->SetVisibility(true);
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())->GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(Cast<AInventoryProjectCharacter>(GetOwner())->GetController())->SetShowMouseCursor(true);
		GetWorld()->GetGameViewport()->SetMouseCaptureMode(EMouseCaptureMode::NoCapture);
		//Getviewport
	}
}

void UInventoryComponent::GetAllItems()
{
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItem::StaticClass(), AllItems);

	for (size_t i = 0; i < AllItems.Num(); i++)
	{
		if (FVector::Dist(AllItems[i]->GetActorLocation(), GetOwner()->GetActorLocation()) <= PickUpDistence)
		{
			AddItemInDistence(Cast<AItem>(AllItems[i]));
			Cast<AItem>(AllItems[i])->SetWidgetVisibility(true);
		}
		else
		{
			DeleteItemNotInDistence(Cast<AItem>(AllItems[i]));
			Cast<AItem>(AllItems[i])->SetWidgetVisibility(false);
		}
	}
}

bool UInventoryComponent::CheckIfAlreadyInArray(AItem* Item)
{
	if (FindIndexOfInDistenceArray(Item) >= 0)
	{
		return true;
	}

	return false;
}

void UInventoryComponent::AddItemInDistence(AItem* Item)
{
	if (CheckIfAlreadyInArray(Item))
		return;

	AllInDistenceItems.Add(Item);
	ConvertItemToOutsideItem();
}

void UInventoryComponent::DeleteItemNotInDistence(AItem* Item)
{
	if (FindIndexOfInDistenceArray(Item) >= 0)
	{
		AllInDistenceItems.RemoveAt(FindIndexOfInDistenceArray(Item));
		ConvertItemToOutsideItem();
	}
}

int32 UInventoryComponent::FindIndexOfInDistenceArray(AItem* Item)
{
	for (size_t i = 0; i < AllInDistenceItems.Num(); i++)
	{
		if (AllInDistenceItems[i] == Item)
		{
			return i;
		}
	}

	return -1;
}

void UInventoryComponent::ConvertItemToOutsideItem()
{
	OutsideItemBox.Empty();

	for (size_t i = 0; i < AllInDistenceItems.Num(); i++)
	{
		if (LeachSameIDItem(AllInDistenceItems[i]->GetItemDetails()))
		{
			;
		}
		else
		{
			FOutsideItem NewOutsideItem;
			NewOutsideItem.ItemDetails = AllInDistenceItems[i]->GetItemDetails();
			NewOutsideItem.Widget = nullptr;
			OutsideItemBox.Add(NewOutsideItem);
		}
	}

	UpdataOutsideBox();
}

bool UInventoryComponent::LeachSameIDItem(FItemDetails Details)
{
	for (size_t i = 0; i < OutsideItemBox.Num(); i++)
	{
		if (OutsideItemBox[i].ItemDetails.ID == Details.ID)
		{
			OutsideItemBox[i].ItemDetails.Num += Details.Num;
			return true;
		}
	}

	return false;
}

void UInventoryComponent::UpdataOutsideBox()
{
	InventoryWidget->ClearWidgetInOutsideBox();

	for (size_t i = 0; i < OutsideItemBox.Num(); i++)
	{
		InventoryWidget->CreateNewOutsideItemWidget(OutsideItemBox[i]);
	}
}

void UInventoryComponent::PickUpByKey()
{
	if (FindClosetItem(-1) == nullptr)
		return;
	AItem* Item = FindClosetItem(-1);

	if (Item->GetItemDetails().Num == 1)
	{
		AddToInsideBox(Item->GetItemDetails());

		Item->Destroy();
		DeleteItemNotInDistence(Item);
	}
	else if (Item->GetItemDetails().Num > 1)
	{
		auto Temp = Item->GetItemDetails();
		Temp.Num -= 1;
		Item->SetItemDetails(Temp);
		ConvertItemToOutsideItem();

		Temp.Num = 1;
		AddToInsideBox(Temp);
	}
}

void UInventoryComponent::PickUpByWidget(UOutsideItemWidget* Widget)
{
	int32 ID;

	for (size_t i = 0; i < OutsideItemBox.Num(); i++)
	{
		if (OutsideItemBox[i].Widget == Widget)
		{
			ID = OutsideItemBox[i].ItemDetails.ID;
		}
	}

	while (FindClosetItem(ID) != nullptr)
	{
		AddToInsideBox(FindClosetItem(ID)->GetItemDetails());
		FindClosetItem(ID)->Destroy();
		DeleteItemNotInDistence(FindClosetItem(ID));
	}
	
	return;
}

AItem* UInventoryComponent::FindClosetItem(int32 ItemID)
{
	double ClosetDistence = PickUpDistence;
	AItem* ClosetItem = nullptr;

	if (ItemID == -1)
	{
		for (size_t i = 0; i < AllInDistenceItems.Num(); i++)
		{
			if (FVector::Dist(AllInDistenceItems[i]->GetActorLocation(), GetOwner()->GetActorLocation()) < ClosetDistence)
			{
				ClosetDistence = FVector::Dist(AllInDistenceItems[i]->GetActorLocation(), GetOwner()->GetActorLocation());
				ClosetItem = AllInDistenceItems[i];
			}
		}
	}
	else
	{
		for (size_t i = 0; i < AllInDistenceItems.Num(); i++)
		{
			if (FVector::Dist(AllInDistenceItems[i]->GetActorLocation(), GetOwner()->GetActorLocation()) < ClosetDistence && AllInDistenceItems[i]->GetItemDetails().ID == ItemID)
			{
				ClosetDistence = FVector::Dist(AllInDistenceItems[i]->GetActorLocation(), GetOwner()->GetActorLocation());
				ClosetItem = AllInDistenceItems[i];
			}
		}
	}

	return ClosetItem;
}

void UInventoryComponent::AddToInsideBox(FItemDetails Details)
{
	if (CheckAlreayInInventory(Details))
		return;

	if (FindIndexCouldSaveItem() == -1)
		return;

	InsideItemBox[FindIndexCouldSaveItem()].ItemDetails = Details;

	InventoryWidget->RefreshInsideItemBox(InsideItemBox);
	UpdataOutsideBox();
}

int32 UInventoryComponent::FindIndexCouldSaveItem()
{
	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].ItemDetails.ID == -1)
			return i;
	}

	return -1;
}

bool UInventoryComponent::CheckAlreayInInventory(FItemDetails Details)
{
	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].ItemDetails.ID == Details.ID)
		{
			InsideItemBox[i].ItemDetails.Num += Details.Num;
			InventoryWidget->RefreshInsideItemBox(InsideItemBox);
			return true;
		}
	}

	return false;
}

void UInventoryComponent::SwitchTwoItemDetail(UInsideItemWidget* OriginalWidget, UInsideItemWidget* TargetWidget)
{
	int32 IndexOfOriginalWidget;
	int32 IndexOfTargetWidget;

	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].Widget == OriginalWidget)
		{
			IndexOfOriginalWidget = i;
		}
		if (InsideItemBox[i].Widget == TargetWidget)
		{
			IndexOfTargetWidget = i;
		}
	}

	auto Temp = InsideItemBox[IndexOfOriginalWidget].ItemDetails;

	InsideItemBox[IndexOfOriginalWidget].ItemDetails = InsideItemBox[IndexOfTargetWidget].ItemDetails;
	InsideItemBox[IndexOfTargetWidget].ItemDetails = Temp;

	InventoryWidget->RefreshInsideItemBox(InsideItemBox);
}

bool UInventoryComponent::DropThisItem(UInsideItemWidget* Widget,int32 DropNum)
{
	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].Widget == Widget)
		{
			auto TempItemDetails = InsideItemBox[i].ItemDetails;
			auto Num = FMath::Clamp(TempItemDetails.Num - DropNum, 0, TempItemDetails.Num);

			if (Num == 0)
			{
				SpawnItem(TempItemDetails);

				InsideItemBox[i].ItemDetails.ID = -1;

				InventoryWidget->RefreshInsideItemBox(InsideItemBox);
				return 0;
			}
			else
			{
				TempItemDetails.Num = FMath::Clamp(DropNum,0, TempItemDetails.Num);
				SpawnItem(TempItemDetails);
				InsideItemBox[i].ItemDetails.Num -= TempItemDetails.Num;

				InventoryWidget->RefreshInsideItemBox(InsideItemBox);
				return 1;
			}
		}
	}

	return 0;
}

void UInventoryComponent::SpawnItem(FItemDetails Details)
{
	if (Details.ID == -1 || Details.Num == 0)
	{
		return;
	}

	AItem* DropedItem = GetWorld()->SpawnActor<AItem>(AItem::StaticClass(), GetOwner()->GetActorTransform());
	DropedItem->SetItemDetails(Details);
	DropedItem->GetMeshComponent()->SetSimulatePhysics(true);
	DropedItem->GetWidgetComponet()->SetWidgetClass(ItemFloatWIdget);
	DropedItem->GetWidgetComponet()->SetWidgetSpace(EWidgetSpace::Screen);
}

FItemDetails UInventoryComponent::GetItemDetails(UInsideItemWidget* Widget)
{
	FItemDetails Empty;

	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].Widget == Widget)
		{
			return InsideItemBox[i].ItemDetails;
		}
	}

	return Empty;
}

void UInventoryComponent::SetItemDetails(UInsideItemWidget* Widget, FItemDetails NewDetails)
{
	for (size_t i = 0; i < InsideItemBox.Num(); i++)
	{
		if (InsideItemBox[i].Widget == Widget)
		{
			InsideItemBox[i].ItemDetails = NewDetails;
		}
	}
}
