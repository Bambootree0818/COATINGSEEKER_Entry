// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PaintMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class COATINGSEEKER_ENTRY_API APaintMenuHUD : public AHUD
{
	GENERATED_BODY()

public: 
	void DisMenu(bool Dis);

	bool IsDis = false;

private:
	UUserWidget* PaintWidget;

protected:
	virtual void BeginPlay() override;
	
};
