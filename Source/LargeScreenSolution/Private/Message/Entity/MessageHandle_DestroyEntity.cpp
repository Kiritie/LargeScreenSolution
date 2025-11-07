// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Entity/MessageHandle_DestroyEntity.h"

#include "ZMQControlTypes.h"
#include "Scene/SceneManager.h"
#include "Message/ZMQMessageManager.h"
#include "Scene/Entity/EntityBase.h"

UMessageHandle_DestroyEntity::UMessageHandle_DestroyEntity()
{
	MessageTag = GameplayTags::Message_DestroyEntity;
}

void UMessageHandle_DestroyEntity::OnReceiveMessage(const FString& InData)
{
	Super::OnReceiveMessage(InData);

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InData);

	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString ID = TEXT("");
		
		if(JsonObject->HasField(TEXT("id")))
		{
			ID = JsonObject->GetStringField(TEXT("id"));
		}

		if(!ASceneManager::Get()->HasSceneObject(ID))
		{
			UZMQMessageManager::SendStatusToServer(202,FString::Printf(TEXT("Spawn Entity is not Exist: id: %s"),*ID),TEXT("DestroyEntity"));
			return;
		}
		
		AEntityBase* Entity = ASceneManager::Get()->GetSceneObject<AEntityBase>(ID);
		if(Entity)
		{
			Entity->Destroy();
			UZMQMessageManager::SendStatusToServer(201,FString::Printf(TEXT("Spawn Entity is Destroy: id: %s"),*ID),TEXT("DestroyEntity"));
			ASceneManager::Get()->RemoveSceneObject(ID);
		}
		else
		{
			UZMQMessageManager::SendStatusToServer(500,FString::Printf(TEXT("Spawn Entity Destroy Failed: id: %s"),*ID),TEXT("DestroyEntity"));
		}
	}
}
