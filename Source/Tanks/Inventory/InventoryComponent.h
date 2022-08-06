// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=("Inventory"), meta=(BlueprintSpawnableComponent) )
class TANKS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	const FInventorySlotInfo* GetItem(int32 SlotIndex) const { return Items.Find(SlotIndex); }

	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item);

	virtual void ClearItem(int32 SlotIndex);

	const TMap<int32, FInventorySlotInfo>& GetItems() const { return Items; }

	int32 GetItemsNum() const { return Items.Num(); }

protected:

	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;
	//FInventorySlotInfo Items;

public:
	UPROPERTY(EditDefaultsOnly)
	UDataTable* StartingItems;
		
};
