// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartingItems)
	{
		Items.Empty();
		auto RowNames = StartingItems->GetRowNames();
		//if (RowNames.Num()>0) UE_LOG(LogTemp, Warning, TEXT("Rows are not empty"));
		int32 SlotIndex=0;
		for (const auto RowName : RowNames)
		{
			const auto Item = StartingItems->FindRow<FInventorySlotInfo>(RowName, "");
			Items.Add(SlotIndex, *Item);
			SlotIndex++;
		}
	}
}

void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
	ClearItem(SlotIndex);
	Items.Add(SlotIndex, Item);
}

void UInventoryComponent::ClearItem(int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}

