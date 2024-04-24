// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "../CPPMaterialProperties.h"
#include "CustomMaterialButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomButtonClickedDelegate, const FMaterialProperties&, MaterialProperties);


UCLASS()
class COATINGSEEKER_ENTRY_API UCustomMaterialButton : public UButton
{
	GENERATED_BODY()

public:
	//マテリアルパラメータ
	FMaterialProperties MaterialProperties;

	FOnCustomButtonClickedDelegate OnCustomButtonClicked;

	UFUNCTION()
	void OnButtonClicked();
	
};
