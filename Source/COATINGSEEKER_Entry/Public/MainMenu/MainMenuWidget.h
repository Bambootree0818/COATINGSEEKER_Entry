// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class COATINGSEEKER_ENTRY_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonPlay;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonQuit;

	// ButtonPlay��OnClicked�C�x���g�Ɋ֘A�Â���
	UFUNCTION()
	void OnButtonPlayClicked();

	// ButtonQuit��OnClicked�C�x���g�Ɋ֘A�Â���
	UFUNCTION()
	void OnButtonQuitClicked();
	
};
