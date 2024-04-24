// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomMaterialButton.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PaintPreview.h"

void UCustomMaterialButton::OnButtonClicked() {

	//UKismetSystemLibrary::PrintString(this, MaterialProperties.Name, true, true, FColor::Cyan, 10.f, TEXT("None"));

	OnCustomButtonClicked.Broadcast(MaterialProperties);
}