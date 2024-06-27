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
	//�g�O�����C�g�{�^��

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight1Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight2Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* UserLight3Button;

	//�Q�[���I���{�^��
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

	// Sphere��OnClicked�C�x���g�Ɋ֘A�Â���
	UFUNCTION()
	void OnButtonSphereClicked();

	// BoxTitle��OnClicked�C�x���g�Ɋ֘A�Â���
	UFUNCTION()
	void OnButtonBoxClicked();

	// Plane��OnClicked�C�x���g�Ɋ֘A�Â���
	UFUNCTION()
	void OnButtonPlaneClicked();

	// MS06R��OnClicked�C�x���g�Ɋ֘A�Â���
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
	//�g�O���{�^���֐�
	UFUNCTION()
	void OnUserLight1ButtonClicked();

	UFUNCTION()
	void OnUserLight2ButtonClicked();

	UFUNCTION()
	void OnUserLight3ButtonClicked();

	void ToggleLight(int LightIndex);

//-----------
	//�h���p�o�[�e�B�J���{�b�N�X
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> MyVerticalBox;

	//json�f�[�^�ǂݍ���
	UFUNCTION(BlueprintCallable)
	void LoadMaterialData();

	//�}�e���A�����ƂɃ{�^���𐶐�
	UFUNCTION()
	void CreateButtonsForMaterials();

	// �{�^���N���b�N���̏����֐�
	UFUNCTION()
	void PaintChange(const FMaterialProperties& MaterialProperties);


	UPROPERTY()
	TArray<FMaterialProperties> MaterialPropertiesArray;

	UPROPERTY()
	TMap<UButton*, FMaterialProperties> ButtonToMaterialMap;

//�Q�[���I���֐�
	UFUNCTION()
	void OnQuitButtonClicked();
	
};


