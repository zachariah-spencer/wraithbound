// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoomSpawnAttempts = 0;
	RoomsToSpawn = 10;
	IsDungeonGenerationComplete = false;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	SpawnStartRoom();
	CheckDungeonSoftLock();
	SpawnNextRoom();
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Spawns the first room in the dungeon
void ADungeonGenerator::SpawnStartRoom()
{
	// Spawn first room of the dungeon
	AActor* OriginRoom =  GetWorld()->SpawnActor<AActor>(OriginRoomBP, GetRootComponent()->GetComponentLocation(), GetRootComponent()->GetComponentRotation());

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

// Checks to ensure the dungeon generation algorithm doesn't get stuck without and valid exit point outlets
void ADungeonGenerator::CheckDungeonSoftLock()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), RoomSpawnAttempts);
	if (RoomSpawnAttempts >= 300)
	{
		UE_LOG(LogTemp, Warning, TEXT("RESETTING LEVEL"));
		UGameplayStatics::OpenLevel(this, FName("WraithboundDungeonLevel"), true);
	}
}

// Finds an available exit point and picks a random room type to spawn at that exit point
void ADungeonGenerator::SpawnNextRoom()
{
    UE_LOG(LogTemp, Warning, TEXT("SpawnNextRoom() Called!"));
    
    // Keeping track of the number of attempts to spawn a room in the case of a soft lock. If this exceeds 300 calls, the map will reset from scratch
    RoomSpawnAttempts++;
    
    // Pick a random exit point from the array of possible exit points
    USceneComponent* SelectedExitPoint = ExitsArray[FMath::RandRange(0, ExitsArray.Num()-1)];

	/* LEGACY CODE:
	 * This was removing showing a room had spawned at a given exit point before checking if the room was spawned in a valid position
	 * As a result, sometimes "exit" points would not be closed up properly, leaving gaps to escape the map from. */
    // ExitsArray.Remove(SelectedExitPoint);
    
    // Pick a random room from the list of possible room configurations to try to spawn next
    TSubclassOf<AActor> SelectedRoom = PossibleRoomsArray[FMath::RandRange(0, PossibleRoomsArray.Num()-1)];
    
    // Spawn the new room and line up its spawn transform with the selected exit point's transform
    AActor* SpawnedRoom = GetWorld()->SpawnActor<AActor>(SelectedRoom, SelectedExitPoint->GetComponentLocation(), SelectedExitPoint->GetComponentRotation());
	
    CheckDungeonSoftLock();

    CheckRoomPositionValid(SpawnedRoom, SelectedExitPoint);
}

void ADungeonGenerator::CheckRoomPositionValid(AActor* LatestSpawnedRoom, USceneComponent* UsedExitPoint)
{
    UE_LOG(LogTemp, Warning, TEXT("CheckRoomsOverlap() Called!"));
	// Populate the overlapping rooms array with any colliding rooms for our next 'if' check
    GetOverlappingRooms(LatestSpawnedRoom);

	// If there is any rooms colliding with where we want to place our next room
	if (!OverlappingRoomsArray.IsEmpty())
	{
		OverlappingRoomsArray.Empty();
		LatestSpawnedRoom->Destroy();
		SpawnNextRoom();
	}
	else
	{
		OverlappingRoomsArray.Empty();
		RoomsToSpawn--;

		// Remove the utilized exit point from the array of possible exit points so it isn't closed up with a wall later
		ExitsArray.Remove(UsedExitPoint);

		USceneComponent* ExitsFolder = Cast<USceneComponent, UActorComponent>(LatestSpawnedRoom->GetComponentsByTag(UActorComponent::StaticClass(), "Exits Folder")[0]);

		TArray<USceneComponent*> Exits;
		ExitsFolder->GetChildrenComponents(false, Exits);
		
		ExitsArray.Append(Exits);

		if (RoomsToSpawn > 0)
		{
			SpawnNextRoom();
		}
		else
		{
			CloseUnusedExits();
			IsDungeonGenerationComplete = true;
			UE_LOG(LogTemp, Warning, TEXT("Dungeon Generation Completed. Number of total attempted room spawns: %d"), RoomSpawnAttempts);
		}
	}
}

void ADungeonGenerator::GetOverlappingRooms(AActor* LatestSpawnedRoom)
{
    UE_LOG(LogTemp, Warning, TEXT("AddOverlappingRoomsToList() Called!"));

	// Get the scene component that contains all the available collision boxes offered by the new room as its children
	USceneComponent* OverlapFolder = Cast<USceneComponent, UActorComponent>(LatestSpawnedRoom->GetComponentsByTag(UActorComponent::StaticClass(), "Overlap Folder")[0]);

	// Get the children components of the OverlapFolder and store them in an array called CollisionBoxes
	TArray<USceneComponent*> CollisionBoxes;
	OverlapFolder->GetChildrenComponents(false, CollisionBoxes);

	// Loop through each collision box
	for(USceneComponent* CollisionBoxInstance : CollisionBoxes){

		// Get any overlapping collision boxes from other room instances
		TArray<UPrimitiveComponent*> OverlappingCollisionBoxes;
		Cast<UBoxComponent>(CollisionBoxInstance)->GetOverlappingComponents(OverlappingCollisionBoxes);

		// Append all the overlapping instances to the OverlappingRoomsArray
		// This will serve as a sum total of any colliding rooms with our current room we're evaluating
		OverlappingRoomsArray.Append(OverlappingCollisionBoxes);
	}
    
}

void ADungeonGenerator::CloseUnusedExits()
{
	UE_LOG(LogTemp, Warning, TEXT("CloseUnusedExits() Called"));

	for (USceneComponent* ExitInstance : ExitsArray)
	{
		GetWorld()->SpawnActor<AActor>(EndWallBP, ExitInstance->GetComponentLocation(), ExitInstance->GetComponentRotation());
	}
}