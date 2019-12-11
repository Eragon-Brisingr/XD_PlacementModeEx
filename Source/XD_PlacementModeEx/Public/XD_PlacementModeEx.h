// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>
#include "XD_PlacementModeSettings.h"

class FXD_PlacementModeExModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;

	static FXD_PlacementModeExModule& Get();

	void RegistAllCategory();
	void UnregistAllCategory();
private:
	void OnPlacementModeRefresh(FName CategoryName);
	void RegistAllItems(FName CategoryName, const TArray<FXD_PlacementModeItem>& PlacealbeItems, TArray<FPlacementModeID>& PlacementItemIds);

	TArray<FXD_PlacementModeCategory> CurrentCategories;
};
