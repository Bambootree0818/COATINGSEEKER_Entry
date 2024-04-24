// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AMainMenuHUD::BeginPlay() {
	
	//WidgetBluePrint��Class���擾����
	FString Path = TEXT("/Game/UI/MainMenu/BPW_MainMenu.BPW_MainMenu_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	//�v���C���[�R���g���[���[���擾
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//WidgetClass��PlayerController���擾�ł���������
	if (WidgetClass && PlayerController) {

		//Widget���쐬
		UUserWidget* UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		//Viewport�ɒǉ�
		UserWidget->AddToViewport(0);

		//MouseCursor��\������
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		PlayerController->SetShowMouseCursor(true);
	}
}
