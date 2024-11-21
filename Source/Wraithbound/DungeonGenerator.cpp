// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoomSpawnAttempts = 0;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::SpawnStartRoom()
{
	// Spawn first room of the dungeon
	AActor* OriginRoom =  GetWorld()->SpawnActor<AActor>(OriginRoomTemplate, GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetComponentRotation());

	// Get the scene component that contains all the available exits offered by the new room as its children
	USceneComponent* ExitsFolder = Cast<USceneComponent, UActorComponent>(OriginRoom->GetComponentsByTag(UActorComponent::StaticClass(), "Exits Folder")[0]);

	// Get the children components of the ExitsFolder and store them in an array called Exits
	TArray<USceneComponent*> Exits;
	ExitsFolder->GetChildrenComponents(false, Exits);
	
	// Append the room's exits to the array of total available exits for the dungeon generator to use
	ExitsArray.Append(Exits);
	
	// Loop through the exits array to verify it is storing the correct information
	// for(USceneComponent* ExitInstance : ExitsArray){
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *ExitInstance->GetName());
	// }
}

void ADungeonGenerator::CheckDungeonSoftLock()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), RoomSpawnAttempts);
	if (RoomSpawnAttempts >= 300)
	{
		UE_LOG(LogTemp, Warning, TEXT("RESETTING LEVEL"));
		UGameplayStatics::OpenLevel(this, FName("WraithboundDungeonLevel"), true);
	}
}

