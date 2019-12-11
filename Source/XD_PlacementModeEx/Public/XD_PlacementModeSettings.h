// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include <UObject/SoftObjectPtr.h>
#include <IPlacementModeModule.h>
#include "XD_PlacementModeSettings.generated.h"

class AActor;

/**
 * 
 */

USTRUCT()
struct FXD_PlacementModeItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UObject> Placeable;
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle = true))
	uint8 bOverride_DisplayName : 1;
	UPROPERTY(EditAnywhere, meta = (editcondition = bOverride_DisplayName))
	FText DisplayName;
};

USTRUCT()
struct FXD_PlacementModeCategory
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FText CategoryDisplayName;
	UPROPERTY(EditAnywhere)
	FName CategoryName;

	UPROPERTY(EditAnywhere, Category = "设置")
	TArray<FXD_PlacementModeItem> Placeables;

	TArray<FPlacementModeID> PlacementModeIDs;
};

UCLASS(Config = "XD_EditorPlacementModeSettings", defaultconfig)
class UXD_PlacementModeSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "设置", meta = (DisplayName = "刷新"))
	uint8 bUpdate : 1;

	UPROPERTY(EditAnywhere, Category = "设置", Config)
	int32 StartPriority = 45;

	UPROPERTY(EditAnywhere, Category = "设置", Config)
	TArray<FXD_PlacementModeCategory> Categories;

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};
