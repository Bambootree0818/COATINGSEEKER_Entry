// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../CPPMaterialProperties.h"
#include "UI/CustomMaterialButton.h"
#include "PaintWidget.generated.h"

class UVerticalBox;
class UButton;

/**
 * 
 */
UCLASS()
class COATINGSEEKER_ENTRY_API UPaintWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//--------
	//トグルライトボタン

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight1Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight2Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight3Button;

	//ゲーム終了ボタン
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

protected:
	//NativecConstruct
	void NativeConstruct() override;

private:
	UPROPERTY()
	int MaterialSlot = 0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Sphere;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Box;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Plane;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MS06R;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts5;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Parts6;

	UPROPERTY()
	TArray<UButton*> HiddenButtons;

	UFUNCTION()
	void ShowHiddenButtons();

	UFUNCTION()
	void HideAllButtons();

	UFUNCTION()
	void SetMSlot0();

	// SphereのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonSphereClicked();

	// BoxTitleのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonBoxClicked();

	// PlaneのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonPlaneClicked();

	// MS06RのOnClickedイベントに関連づける
	UFUNCTION()
	void OnButtonMS06RClicked();

	UFUNCTION()
	void OnButtonParts1Clicked();
	UFUNCTION()
	void OnButtonParts2Clicked();
	UFUNCTION()
	void OnButtonParts3Clicked();
	UFUNCTION()
	void OnButtonParts4Clicked();
	UFUNCTION()
	void OnButtonParts5Clicked();
	UFUNCTION()
	void OnButtonParts6Clicked();

	//-----
	//トグルボタン関数
	UFUNCTION()
	void OnUserLight1ButtonClicked();

	UFUNCTION()
	void OnUserLight2ButtonClicked();

	UFUNCTION()
	void OnUserLight3ButtonClicked();

	void ToggleLight(int LightIndex);

//-----------
	//塗料用バーティカルボックス
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> MyVerticalBox;

	//jsonデータ読み込み
	UFUNCTION(BlueprintCallable)
	void LoadMaterialData();

	//マテリアルごとにボタンを生成
	UFUNCTION()
	void CreateButtonsForMaterials();

	// ボタンクリック時の処理関数
	UFUNCTION()
	void PaintChange(const FMaterialProperties& MaterialProperties);


	UPROPERTY()
	TArray<FMaterialProperties> MaterialPropertiesArray;

	UPROPERTY()
	TMap<UButton*, FMaterialProperties> ButtonToMaterialMap;

//ゲーム終了関数
	UFUNCTION()
	void OnQuitButtonClicked();
	
};


