// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ActorSpawnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UActorSpawnerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnerClass;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* DraggedText;

	UPROPERTY(EditAnywhere)
	FString DraggedName;

	UPROPERTY()
	AActor* SpawnedActor;
	
	UPROPERTY()
	APlayerController* PlayerController;

	void OnMouseButtonUp();
};
