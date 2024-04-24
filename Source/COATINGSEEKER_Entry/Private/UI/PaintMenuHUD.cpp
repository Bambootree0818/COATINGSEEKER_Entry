// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PaintMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void APaintMenuHUD::BeginPlay() {

	//PaintWidget�̃N���X���擾
	FString Path = TEXT("/Game/UI/PaintMenu/BPW_PaintMenu.BPW_PaintMenu_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (WidgetClass && PlayerController) {
		PaintWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		PaintWidget->SetVisibility(ESlateVisibility::Collapsed);

		PaintWidget->AddToViewport(0);

		// MouseCursor��\������
		//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		//PlayerController->SetShowMouseCursor(true);
	}

	//UKismetSystemLibrary::PrintString(this, "widget�쐬", true, true, FColor::Cyan, 2.f, TEXT("None"));
}

void APaintMenuHUD::DisMenu(bool Dis) {
	//PlayerController���擾
	APlayerController* PlayerController = GetOwningPlayerController();

	if (Dis == false) {

		// Pause���j���[��\������
		PaintWidget->SetVisibility(ESlateVisibility::Visible);

		// UI���[�h�ɐݒ肷��
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, PaintWidget, EMouseLockMode::DoNotLock, false);

		// Game��Pause��Ԃɂ���
		//UGameplayStatics::SetGamePaused(GetWorld(), true);

		// Mouse�J�[�\����\������
		PlayerController->SetShowMouseCursor(true);

		IsDis = true;
	}
	else
	{

		// Game��Pause��Ԃ���������
		//UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Mouse�J�[�\�����\���ɂ���
		PlayerController->SetShowMouseCursor(false);

		// Game��Input��Ԃɖ߂�
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);

		// Pause���j���[��܂肽���ݏ�Ԃɂ���
		PaintWidget->SetVisibility(ESlateVisibility::Collapsed);

		IsDis = false;
	}
}