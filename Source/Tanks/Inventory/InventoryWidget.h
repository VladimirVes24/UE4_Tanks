// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventoryCellWidget;

UCLASS()
class TANKS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init(int32 ItemsCount);

	bool AddItem(const FInventorySlotInfo& InSlot, const FInventoryItemInfo& Info, int32 SlotIndex);

	FOnItemDrop OnItemDrop;

protected:

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel* CellsPanel;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UInventoryCellWidget* GoldCell;

	UPROPERTY()
	TArray<UInventoryCellWidget*> CellWidgets;

	UInventoryCellWidget* CreateCell();

	void OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To);
	
	
	
	
	
};
