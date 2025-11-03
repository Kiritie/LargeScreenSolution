// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/Entity/DynamicEntityBase.h"

#include "../../../../../Plugins/MeshLoader/Source/MeshLoader/Public/MeshLoaderActor.h"

// void ADynamicEntityBase::LoadMeshAndTextureData(const TArray<FModelData>& ModelData)
// {
// 	for (const FModelData& Data : ModelData)
// 	{
// 		FActorSpawnParameters SpawnParameters;
// 		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
// 		AMeshLoaderActor* Loader = GetWorld()->SpawnActor<AMeshLoaderActor>(AMeshLoaderActor::StaticClass(), GetActorTransform(), SpawnParameters);
// 		if (Loader)
// 		{
// 			Loader->LoadMeshAndTextureData(Data);
// 		}
// 	}
// }
