// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/InGameGameMode.h"
#include "UI/PaintMenuHUD.h"
#include "PaintPreview.h"
#include "Framework/InGamePlayerController.h"

AInGameGameMode::AInGameGameMode() {

	DefaultPawnClass = APaintPreview::StaticClass();

	HUDClass = APaintMenuHUD::StaticClass();

	PlayerControllerClass = AInGamePlayerController::StaticClass();
}

