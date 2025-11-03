// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshLoaderStatics.h"
#include "Scene/Entity/EntityBase.h"
#include "DynamicEntityBase.generated.h"

/**
 * 
 */
UCLASS()
class LARGESCREENSOLUTION_API ADynamicEntityBase : public AEntityBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void LoadMeshAndTextureData(const TArray<FModelData>& ModelData);
};
