// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Entity/MessageHandle_SpawnEntity.h"

#include "ZMQControlTypes.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/EntityBase.h"

#include "Message/ZMQMessageManager.h"
#include "Core/ZMQCoreManager.h"
#include "ZMQControlTypes.h"

UMessageHandle_SpawnEntity::UMessageHandle_SpawnEntity()
{
	MessageTag = GameplayTags::Message_SpawnEntity;
}

void UMessageHandle_SpawnEntity::OnReceiveMessage(const FString& InData)
{
	Super::OnReceiveMessage(InData);

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InData);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ID = TEXT("");
		UClass* Class = nullptr;
		FVector Location = FVector::ZeroVector;
		FRotator Rotation = FRotator::ZeroRotator;
		FVector Scale = FVector::ZeroVector;
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}

		if(ASceneManager::Get()->HasSceneObject(ID))
		{
			UZMQMessageManager::SendStatusToServer(202,FString::Printf(TEXT("Spawn Entity is Exist: id: %s"),*ID),TEXT("SpawnEntity"));
			return;
		}

		if(JsonObject->HasField(TEXT("class")))
		{
			const FString Field = JsonObject->GetStringField(TEXT("class"));
			Class = LoadClass<AActor>(nullptr, *Field);
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
		
		if (Class)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AEntityBase* Entity = GetWorld()->SpawnActor<AEntityBase>(Class, SpawnInfo);
			if(Entity)
			{
				Entity->SetActorLocation(Location);
				Entity->SetActorRotation(Rotation);
				Entity->SetActorScale3D(Scale);

				ASceneManager::Get()->AddSceneObject(ID, Entity);

				UZMQMessageManager::SendStatusToServer(201, FString::Printf(TEXT("Spawn Created Entity Success: id: %s"),*ID),TEXT("SpawnEntity"));
			}
			else
			{
				UZMQMessageManager::SendStatusToServer(500,FString::Printf(TEXT("Spawn Entity Failed: id: %s"),*ID),TEXT("SpawnEntity"));
			}
		}
	}
}
