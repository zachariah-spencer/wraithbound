// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class WRAITHBOUND_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnStartRoom();
	
	UFUNCTION(BlueprintCallable)
	void CheckDungeonSoftLock();

	UFUNCTION(BlueprintCallable)
	void SpawnNextRoom();

	UFUNCTION(BlueprintCallable)
	void CheckRoomPositionValid(AActor* LatestSpawnedRoom, USceneComponent* UsedExitPoint);

	UFUNCTION(BlueprintCallable)
	void GetOverlappingRooms(AActor* LatestSpawnedRoom);

	UFUNCTION(BlueprintCallable)
	void CloseUnusedExits();

	UPROPERTY(EditAnywhere, Category= "Dungeon Generator")
	TSubclassOf<AActor> OriginRoomBP;

	UPROPERTY(EditAnywhere, Category= "Dungeon Generator")
	TSubclassOf<AActor> EndWallBP;
	
	UPROPERTY(EditAnywhere, Category= "Dungeon Generator")
	TArray<TSubclassOf<AActor>> PossibleRoomsArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Dungeon Generator")
	TArray<USceneComponent*> ExitsArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Dungeon Generator")
	TArray<USceneComponent*> OverlappingRoomsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Dungeon Generator")
	int RoomSpawnAttempts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Dungeon Generator")
	int RoomsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Dungeon Generator")
	bool IsDungeonGenerationComplete;

};
