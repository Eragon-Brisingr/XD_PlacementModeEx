// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "XD_PlacementModeEx.h"
#include "XD_PlacementModeSettings.h"
#include "ISettingsModule.h"
#include <AssetRegistryModule.h>
#include "IAssetRegistry.h"

#define LOCTEXT_NAMESPACE "FXD_PlacementModeExModuleExModule"

namespace XD_PlacementModeSettingsConfig
{
	const FName ContainerName = TEXT("Project");
	const FName CategoryName = TEXT("Plugins");
	const FName SectionName = TEXT("XD_PlacementModeSettings");
}

void FXD_PlacementModeExModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	check(IPlacementModeModule::IsAvailable());

	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	PlacementModeModule.OnPlacementModeCategoryRefreshed().AddRaw(this, &FXD_PlacementModeExModule::OnPlacementModeRefresh);

	RegistAllCategory();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(XD_PlacementModeSettingsConfig::ContainerName, XD_PlacementModeSettingsConfig::CategoryName, XD_PlacementModeSettingsConfig::SectionName,
			LOCTEXT("XD_PlacementModeSettings", "配置放置模式扩展面板"),
			LOCTEXT("XD_PlacementModeSettingsDescription", "配置放置模式扩展面板"),
			GetMutableDefault<UXD_PlacementModeSettings>()
		);
	}
}

void FXD_PlacementModeExModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (IPlacementModeModule::IsAvailable())
	{
		IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
		PlacementModeModule.OnPlacementModeCategoryRefreshed().RemoveAll(this);

		UnregistAllCategory();
	}

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(XD_PlacementModeSettingsConfig::ContainerName, XD_PlacementModeSettingsConfig::CategoryName, XD_PlacementModeSettingsConfig::SectionName);
	}
}

FXD_PlacementModeExModule& FXD_PlacementModeExModule::Get()
{
	return FModuleManager::LoadModuleChecked<FXD_PlacementModeExModule>("XD_PlacementModeEx");
}

void FXD_PlacementModeExModule::RegistAllCategory()
{
	const UXD_PlacementModeSettings* PlacementModeSettings = GetDefault<UXD_PlacementModeSettings>();
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	CurrentCategories = PlacementModeSettings->Categories;
	for (int32 Idx = 0; Idx < CurrentCategories.Num(); ++Idx)
	{
		const FXD_PlacementModeCategory& Category = CurrentCategories[Idx];
		PlacementModeModule.RegisterPlacementCategory(FPlacementCategoryInfo(Category.CategoryDisplayName, Category.CategoryName, Category.CategoryName.ToString(), PlacementModeSettings->StartPriority + Idx));
	}
}

void FXD_PlacementModeExModule::UnregistAllCategory()
{
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	for (const FXD_PlacementModeCategory& Category : CurrentCategories)
	{
		for (const FPlacementModeID& PlacementItemId : Category.PlacementModeIDs)
		{
			PlacementModeModule.UnregisterPlaceableItem(PlacementItemId);
		}
		PlacementModeModule.UnregisterPlacementCategory(Category.CategoryName);
	}
	CurrentCategories.Empty();
}

void FXD_PlacementModeExModule::OnPlacementModeRefresh(FName CategoryName)
{
	const UXD_PlacementModeSettings* PlacementModeSettings = GetDefault<UXD_PlacementModeSettings>();
	for (FXD_PlacementModeCategory& Category : CurrentCategories)
	{
		if (Category.CategoryName == CategoryName)
		{
			RegistAllItems(CategoryName, Category.Placeables, Category.PlacementModeIDs);
			break;
		}
	}
}

void FXD_PlacementModeExModule::RegistAllItems(FName CategoryName, const TArray<FXD_PlacementModeItem>& PlacealbeItems, TArray<FPlacementModeID>& PlacementItemIds)
{
	IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
	IAssetRegistry& AssetRegistry = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	for (const FPlacementModeID& PlacementItemId : PlacementItemIds)
	{
		PlacementModeModule.UnregisterPlaceableItem(PlacementItemId);
	}
	PlacementItemIds.Empty();
	for (const FXD_PlacementModeItem& PlacealbeItem : PlacealbeItems)
	{
		FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(*PlacealbeItem.Placeable.ToSoftObjectPath().GetAssetPathString());
		if (AssetData.IsValid())
		{
			FPlaceableItem* PlaceableItem = new FPlaceableItem(nullptr, AssetData, NAME_None);
			if (PlacealbeItem.bOverride_DisplayName)
			{
				PlaceableItem->DisplayName = PlacealbeItem.DisplayName;
			}
			TOptional<FPlacementModeID> PlacementModeID = PlacementModeModule.RegisterPlaceableItem(CategoryName, MakeShareable(PlaceableItem));
			if (PlacementModeID)
			{
				PlacementItemIds.Add(PlacementModeID.GetValue());
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXD_PlacementModeExModule, XD_PlacementModeEx)
