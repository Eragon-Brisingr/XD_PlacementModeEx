// Fill out your copyright notice in the Description page of Project Settings.


#include "XD_PlacementModeSettings.h"
#include "XD_PlacementModeEx.h"

void UXD_PlacementModeSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UXD_PlacementModeSettings, bUpdate))
	{
		bUpdate = false;

		FXD_PlacementModeExModule& PlacementModeEx = FXD_PlacementModeExModule::Get();
		PlacementModeEx.UnregistAllCategory();
		PlacementModeEx.RegistAllCategory();
	}
}
