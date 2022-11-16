// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UWidgetComponent;
class UUserWidget;

struct FItemDetails
{
	int32 ID = -1;
	int32 Num = 0;

	FText ItemName = FText::FromString("None");
	FText ItemDiscription = FText::FromString("None");;
	UTexture2D* ItemIcon = nullptr;
	UStaticMesh* Mesh = nullptr;
};

UCLASS()
class INVENTORYPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	void SetItemDetails(FItemDetails NewDetails);
	FItemDetails GetItemDetails();
	
	void SetWidgetVisibility(bool NewValue);
	UStaticMeshComponent* GetMeshComponent() { return MeshComponent;};
	UWidgetComponent* GetWidgetComponet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Num;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDiscription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;
};
