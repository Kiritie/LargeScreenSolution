// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SceneManager.generated.h"

UCLASS(Blueprintable)
class LARGESCREENSOLUTION_API ASceneManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneManager();

	static ASceneManager* Get();

private:
	static ASceneManager* Instance;

public:
	/**
	* 当初始化
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInitialize")
	void K2_OnInitialize();
	UFUNCTION()
	virtual void OnInitialize();
	/**
	* 当刷新
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnRefresh")
	void K2_OnRefresh(float DeltaSeconds);
	UFUNCTION()
	virtual void OnRefresh(float DeltaSeconds);
	/**
	* 当销毁
	*/
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnTermination")
	void K2_OnTermination();
	UFUNCTION()
	virtual void OnTermination();

public:
	UFUNCTION(BlueprintCallable)
	void AddSceneObject(const FString& InID, UObject* InObject);

	UFUNCTION(BlueprintCallable)
	void RemoveSceneObject(const FString& InID);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, UObject*> SceneObjectMap;

public:
	UFUNCTION(BlueprintPure)
	bool HasSceneObject(const FString& InID) const;

	UFUNCTION(BlueprintPure)
	UObject* GetSceneObject(const FString& InID) const;

	template<class T>
	T* GetSceneObject(const FString& InID) const
	{
		return Cast<T>(GetSceneObject(InID));
	}

	UFUNCTION(BlueprintPure)
	TArray<UObject*> GetSceneObjects() const;
};
