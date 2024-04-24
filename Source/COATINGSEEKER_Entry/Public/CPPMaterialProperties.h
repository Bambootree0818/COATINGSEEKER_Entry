// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPMaterialProperties.generated.h"

USTRUCT(BlueprintType)
struct FMaterialProperties
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FVector BaseColor;

	UPROPERTY(BlueprintReadOnly)
	float Metallic;

	UPROPERTY(BlueprintReadOnly)
	float Roughness;

	UPROPERTY(BlueprintReadOnly)
	float Specular;

	UPROPERTY(BlueprintReadOnly)
	float ClearCoat;

	UPROPERTY(BlueprintReadOnly)
	float ClearCoatGloss;

	UPROPERTY(BlueprintReadOnly)
	int32 Matallic_type;
};
