// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/Entity/DynamicEntityBase.h"

#include "MeshLoaderActor.h"

void ADynamicEntityBase::OnInitialize()
{
	Super::OnInitialize();
}

void ADynamicEntityBase::OnRefresh(float DeltaSeconds)
{
	Super::OnRefresh(DeltaSeconds);
}

void ADynamicEntityBase::OnTermination()
{
	Super::OnTermination();

	for (auto Actor : MeshLoaderActors)
	{
		Actor->Destroy();
	}
	MeshLoaderActors.Empty();
}

void ADynamicEntityBase::LoadMeshAndTextureData(const TArray<FModelData>& ModelData)
{
	UClass* Class = LoadClass<AMeshLoaderActor>(nullptr, TEXT("/Script/Engine.Blueprint'/MeshLoader/Example/Blueprints/BP_MeshLoaderActor.BP_MeshLoaderActor_C'"));
	
	for (const FModelData& Data : ModelData)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMeshLoaderActor* Loader = GetWorld()->SpawnActor<AMeshLoaderActor>(Class, GetActorTransform(), SpawnParameters);
		if (Loader)
		{
			Loader->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Loader->LoadMeshAndTextureData(Data);
		}
	}
}
