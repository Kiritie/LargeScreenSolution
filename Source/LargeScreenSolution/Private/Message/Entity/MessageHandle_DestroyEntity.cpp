// Fill out your copyright notice in the Description page of Project Settings.

#include "Message/Entity/MessageHandle_DestroyEntity.h"

#include "ZMQControlTypes.h"
#include "Scene/SceneManager.h"
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
		
		AEntityBase* Entity = ASceneManager::Get()->GetSceneObject<AEntityBase>(ID);
		if(Entity)
		{
			Entity->Destroy();

			ASceneManager::Get()->RemoveSceneObject(ID);
		}
	}
}
