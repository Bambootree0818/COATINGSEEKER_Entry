// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InGamePlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "UI/PaintMenuHUD.h"


AInGamePlayerController::AInGamePlayerController()
{
	// Input Action「IA_Pause」を読み込む
	PauseAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Action/IA_Menu"));
}

void AInGamePlayerController::BeginPlay()
{
	// 入力をGameのみにする
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this, false);

	// マウスを非表示にする
	SetShowMouseCursor(false);
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AInGamePlayerController::DispPause);
	}
}

void AInGamePlayerController::DispPause(const FInputActionValue& Value)
{
	//UKismetSystemLibrary::PrintString(this, "ooooooooo", true, true, FColor::Cyan, 10.f, TEXT("None"));
	// inputのValueはboolに変換できる
	if (const bool V = Value.Get<bool>())
	{
		if (APaintMenuHUD* HUD = Cast<APaintMenuHUD>(GetHUD()))
		{
			// Pauseメニューの表示/非表示を実行する
			HUD->DisMenu(HUD->IsDis);
		}
	}
}