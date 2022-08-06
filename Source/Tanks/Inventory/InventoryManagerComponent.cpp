// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.h"
#include "InventoryCellWidget.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
	//
}

void UInventoryManagerComponent::Init(UInventoryComponent* InventoryComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Entered UInventoryManagerComponent::Init"));
	LocalInventoryComponent = InventoryComponent;
	
	if (LocalInventoryComponent && InventoryWidgetClass)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Entered UInventoryManagerComponent::Init 1st Condition"));
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(MinInventorySize,LocalInventoryComponent->GetItemsNum()));
		InventoryWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);

		for (const auto& Item : LocalInventoryComponent->GetItems())
		{
			if (auto* Data = GetItemData(Item.Value.Id))
			{
				if (InventoryWidget->AddItem(Item.Value, *Data, Item.Key)) UE_LOG(LogTemp, Warning, TEXT("Successfully added item"));
			}
		}
	}	
}

const FInventoryItemInfo* UInventoryManagerComponent::GetItemData(const FName& InID) const
{
	return ItemsData ? ItemsData->FindRow<FInventoryItemInfo>(InID, "") : nullptr;
}

void UInventoryManagerComponent::OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	
	FInventorySlotInfo FromItem = From->GetItem();
	FInventorySlotInfo ToItem = To->GetItem();
	
	if (FromItem.Id == FName("Gold"))
	{
		return;
	}

	From->Clear();
	To->Clear();

	To->AddItem(FromItem, *GetItemData(FromItem.Id));

	if(!ToItem.Id.IsNone())
	{
		From->AddItem(ToItem, *GetItemData(ToItem.Id));
	}
}



