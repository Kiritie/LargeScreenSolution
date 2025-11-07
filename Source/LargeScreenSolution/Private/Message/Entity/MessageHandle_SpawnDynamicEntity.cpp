// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Entity/MessageHandle_SpawnDynamicEntity.h"

#include "ZMQControlTypes.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/EntityBase.h"

#include "Message/ZMQMessageManager.h"
#include "Core/ZMQCoreManager.h"
#include "ZMQControlTypes.h"
#include "MeshLoaderStatics.h"
#include "Scene/Entity/DynamicEntityBase.h"

UMessageHandle_SpawnDynamicEntity::UMessageHandle_SpawnDynamicEntity()
{
	MessageTag = GameplayTags::Message_SpawnDynamicEntity;
}

void UMessageHandle_SpawnDynamicEntity::OnReceiveMessage(const FString& InData)
{
	Super::OnReceiveMessage(InData);

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InData);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ID = TEXT("");
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		FVector Scale = FVector::ZeroVector;
		FString Data = TEXT("");
		FString ModelPath = TEXT("");
		FString ConfigPath = TEXT("");
		TArray<FModelData> ModelData;
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}

		if(ASceneManager::Get()->HasSceneObject(ID))
		{
			UZMQMessageManager::SendStatusToServer(202,FString::Printf(TEXT("Spawn Entity is Exist: id: %s"),*ID),TEXT("SpawnDynamicEntity"));
			return;
		}

		if(JsonObject->HasField(TEXT("location")))
		{
			const TSharedPtr<FJsonObject> Location_Obj = JsonObject->GetObjectField(TEXT("location"));
			Location.X = Location_Obj->GetNumberField(TEXT("x"));
			Location.Y = Location_Obj->GetNumberField(TEXT("y"));
			Location.Z = Location_Obj->GetNumberField(TEXT("z"));
		}
		if(JsonObject->HasField(TEXT("rotation")))
		{
			const TSharedPtr<FJsonObject> Rotation_Obj = JsonObject->GetObjectField(TEXT("rotation"));
			Rotation.Roll = Rotation_Obj->GetNumberField(TEXT("roll"));
			Rotation.Pitch = Rotation_Obj->GetNumberField(TEXT("pitch"));
			Rotation.Yaw = Rotation_Obj->GetNumberField(TEXT("yaw"));
		}
		if(JsonObject->HasField(TEXT("scale")))
		{
			const TSharedPtr<FJsonObject> Scale_Obj = JsonObject->GetObjectField(TEXT("scale"));
			Scale.X = Scale_Obj->GetNumberField(TEXT("x"));
			Scale.Y = Scale_Obj->GetNumberField(TEXT("y"));
			Scale.Z = Scale_Obj->GetNumberField(TEXT("z"));
		}

		if(JsonObject->HasField(TEXT("data")))
		{
			const TSharedPtr<FJsonObject> Scale_Obj = JsonObject->GetObjectField(TEXT("data"));
			if (Scale_Obj.IsValid())
			{
				// 创建一个 JSON Writer
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Data);
				// 序列化 FJsonObject
				FJsonSerializer::Serialize(Scale_Obj.ToSharedRef(), Writer);
			}
		}
		
		if(JsonObject->HasField(TEXT("model_path")))
		{
			ModelPath = JsonObject->GetStringField(TEXT("model_path"));
		}
		
		if (UMeshLoaderStatics::LoadModelData(ModelPath, ModelData))
		{
			UZMQMessageManager::SendStatusToServer(201, TEXT("Config Resolve is Success"),TEXT("SpawnDynamicEntity"));
		}
		else
		{
			UZMQMessageManager::SendStatusToServer(500, TEXT("Config Resolve is Failed"),TEXT("SpawnDynamicEntity"));
		}
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ADynamicEntityBase* Entity = GetWorld()->SpawnActor<ADynamicEntityBase>(ADynamicEntityBase::StaticClass(), SpawnInfo);
		
		if(Entity)
		{
			Entity->SetActorLocation(Location);
			Entity->SetActorRotation(Rotation);
			Entity->SetActorScale3D(Scale);
			Entity->LoadData(Data);
			Entity->LoadMeshAndTextureData(ModelData);

			ASceneManager::Get()->AddSceneObject(ID, Entity);

			UZMQMessageManager::SendStatusToServer(201, FString::Printf(TEXT("Spawn Created Entity Success: id: %s"),*ID),TEXT("SpawnDynamicEntity"));
		}
		else
		{
			UZMQMessageManager::SendStatusToServer(500,FString::Printf(TEXT("Spawn Dynamic Entity Failed: id: %s"),*ID),TEXT("SpawnDynamicEntity"));
		}
	}
}
