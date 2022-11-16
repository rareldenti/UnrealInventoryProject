// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemActor/Item.h"
#include "Components/WidgetComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");

	SetRootComponent(MeshComponent);
	WidgetComponent->SetupAttachment(MeshComponent);
}

void AItem::SetItemDetails(FItemDetails NewDetails)
{
	this->ID = NewDetails.ID;
	this->Num = NewDetails.Num;
	this->ItemName = NewDetails.ItemName;
	this->ItemDiscription = NewDetails.ItemDiscription;
	this->ItemIcon = NewDetails.ItemIcon;
	MeshComponent->SetStaticMesh(NewDetails.Mesh);
}

FItemDetails AItem::GetItemDetails()
{
	FItemDetails SelfDetails;

	SelfDetails.ID = this->ID;
	SelfDetails.Num = this->Num;
	SelfDetails.ItemName = this->ItemName;
	SelfDetails.ItemDiscription = this->ItemDiscription;
	SelfDetails.ItemIcon = this->ItemIcon;
	SelfDetails.Mesh = MeshComponent->GetStaticMesh();

	return SelfDetails;
}

void AItem::SetWidgetVisibility(bool NewValue)
{
	WidgetComponent->SetVisibility(NewValue);
}

UWidgetComponent* AItem::GetWidgetComponet()
{
	return WidgetComponent;
}