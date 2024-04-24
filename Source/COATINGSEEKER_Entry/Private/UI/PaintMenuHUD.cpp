// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PaintMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void APaintMenuHUD::BeginPlay() {

	//PaintWidgetのクラスを取得
	FString Path = TEXT("/Game/UI/PaintMenu/BPW_PaintMenu.BPW_PaintMenu_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (WidgetClass && PlayerController) {
		PaintWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		PaintWidget->SetVisibility(ESlateVisibility::Collapsed);

		PaintWidget->AddToViewport(0);

		// MouseCursorを表示する
		//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		//PlayerController->SetShowMouseCursor(true);
	}

	//UKismetSystemLibrary::PrintString(this, "widget作成", true, true, FColor::Cyan, 2.f, TEXT("None"));
}

void APaintMenuHUD::DisMenu(bool Dis) {
	//PlayerControllerを取得
	APlayerController* PlayerController = GetOwningPlayerController();

	if (Dis == false) {

		// Pauseメニューを表示する
		PaintWidget->SetVisibility(ESlateVisibility::Visible);

		// UIモードに設定する
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, PaintWidget, EMouseLockMode::DoNotLock, false);

		// GameをPause状態にする
		//UGameplayStatics::SetGamePaused(GetWorld(), true);

		// Mouseカーソルを表示する
		PlayerController->SetShowMouseCursor(true);

		IsDis = true;
	}
	else
	{

		// GameのPause状態を解除する
		//UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Mouseカーソルを非表示にする
		PlayerController->SetShowMouseCursor(false);

		// GameのInput状態に戻す
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);

		// Pauseメニューを折りたたみ状態にする
		PaintWidget->SetVisibility(ESlateVisibility::Collapsed);

		IsDis = false;
	}
}