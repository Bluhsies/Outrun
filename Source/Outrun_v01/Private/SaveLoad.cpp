// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoad.h"
#include "Kismet/GameplayStatics.h"
#include "SaveMaster.h"
#include "HealthCollectable.h"
#include "ScoreCollectable.h"

ASaveLoad::ASaveLoad()
{

	PlayerName = TEXT("Default");
	UserSlot = 0;

	CollectableName = TEXT("Default");
	CollectableSlot = 0;

}

/*
* The SaveGame function creates an instance of SaveMaster, which utilises GameplayStatics to create a 
* save game object. This instance of SaveMaster will store the parameters which need to be kept
* in similar containers that are outlined in the SaveLoad class, but will by saving them in a slot,
* it allows the user to close the game and use the load function to put all actors and parameters
* within the scene at the point of the save occurring.
* Each parameter (except for the players) which needs to be saved, is iterated through a for loop
* to save the parameters of each actor which is assigned to the array in the inspector. To make it
* more streamline, a number of structs were created in SaveMaster which are called in the loops, without the
* need to make more individual parameters, as most of them use the same data (position and rotation) to be
* saved.
* For the players, these do not interate through a for loop, as there is only one instance of each of these
* actors.
* At the very end of the function, the data stored within the instance of SaveData is saved to a slot, using
* the PlayerName and UserSlot populated in the constructor. 
*/

void ASaveLoad::SaveGame()
{

	USaveMaster* SaveData = Cast<USaveMaster>(UGameplayStatics::CreateSaveGameObject(USaveMaster::StaticClass()));
	
	
	SaveData->PlayerName = PlayerName;
	SaveData->UserSlot = UserSlot;
	SaveData->CollectableName = CollectableName;
	SaveData->CollectableSlot = CollectableSlot;


	for (AHealthCollectable* Collectable : HealthCollectableArray)
	{

		if (Collectable)
		{

			FTransform HealthCollectableTransform = Collectable->GetActorTransform();
			FActorTransforms TransformData;
			TransformData.ActorPosition = HealthCollectableTransform.GetLocation();
			TransformData.ActorRotation = HealthCollectableTransform.GetRotation().Rotator();

			SaveData->ActorPosRot.Add(TransformData);

		}

	}

	for (AScoreCollectable* ScoreCollectable : ScoreCollectableArray)
	{

		if (ScoreCollectable)
		{

			FTransform ScoreCollectableTransform = ScoreCollectable->GetActorTransform();
			FActorTransforms TransformData;
			TransformData.ActorPosition = ScoreCollectableTransform.GetLocation();
			TransformData.ActorRotation = ScoreCollectableTransform.GetRotation().Rotator();

			SaveData->ColActorPosRot.Add(TransformData);

		}

	}

	for (AHazard* Hazards : HazardArray)
	{

		if (Hazards)
		{

			FTransform HazardTransform = Hazards->GetActorTransform();
			FActorTransforms TransformData;
			TransformData.ActorPosition = HazardTransform.GetLocation();
			TransformData.ActorRotation = HazardTransform.GetRotation().Rotator();

			SaveData->HazActorPosRot.Add(TransformData);

		}

	}

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainCharacter)
	{
		FTransform PlayerTransform = MainCharacter->GetActorTransform();
		FMoveableActorTransforms TransformData;

		TransformData.Health = MainCharacter->GetHealth();
		TransformData.Score = MainCharacter->GetScore();

		TransformData.MoveableActorPosition = MainCharacter->GetActorLocation();
		TransformData.MoveableActorRotation = MainCharacter->GetActorRotation();

		SaveData->PlayerOneData = TransformData;

	}

	AMainCharacter* SecondCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));
	if (SecondCharacter)
	{
		FTransform PlayerTransform = SecondCharacter->GetActorTransform();
		FMoveableActorTransforms TransformData;

		TransformData.Health = SecondCharacter->GetHealth();
		TransformData.Score = SecondCharacter->GetScore();

		TransformData.MoveableActorPosition = SecondCharacter->GetActorLocation();
		TransformData.MoveableActorRotation = SecondCharacter->GetActorRotation();

		SaveData->PlayerTwoData = TransformData;

	}

	for (AEnemy* Enemy : EnemyArray)
	{

		if (Enemy)
		{

			FTransform EnemyTransform = Enemy->GetActorTransform();
			FActorTransforms TransformData;
			TransformData.ActorPosition = EnemyTransform.GetLocation();
			TransformData.ActorRotation = EnemyTransform.GetRotation().Rotator();

			SaveData->EnemyPosRot.Add(TransformData);

		}

	}

	UE_LOG(LogTemp, Warning, TEXT("SaveWorking"));
	UGameplayStatics::SaveGameToSlot(SaveData, PlayerName, UserSlot);

}

/*
* The LoadGame function is similar to the SaveGame function, but creates an instance of SaveMaster
* which takes data from the SaveGameToSlot function and loads it from the slot instead, using the
* player name and user slot to identify which one needs to be loaded.
* We first check to see if LoadData exists, because if there is no saved data to begin with, this
* could cause issues with trying to load null pointers.
* Similarly to the SaveGame function, but with loading instead, each instance of loading the data 
* is iterated through a for loop to assign the parameters to the objects which are still available in the
* array from SaveMaster. For example, the first for loop will iterate through each actor's data
* saved within ActorPosRot and then assign it to the relevant actor which it was saved against originally
* when the SaveGame function is called.
* Again, with the player characters, these are not iterated through a for loop as there is only
* one instance of each of these actors in the scene.
*/

void ASaveLoad::LoadGame()
{

	USaveMaster* LoadData = Cast<USaveMaster>(UGameplayStatics::LoadGameFromSlot(PlayerName, UserSlot));

	if (LoadData)
	{

		for (int32 i = 0; i < LoadData->ActorPosRot.Num(); ++i)
		{

			if (HealthCollectableArray.IsValidIndex(i) && HealthCollectableArray[i])
			{

				const FActorTransforms& TransformData = LoadData->ActorPosRot[i];
				HealthCollectableArray[i]->SetActorLocation(TransformData.ActorPosition);
				HealthCollectableArray[i]->SetActorRotation(TransformData.ActorRotation);

			}

			
		}

		for (int32 i = 0; i < LoadData->ColActorPosRot.Num(); ++i)
		{
			if (ScoreCollectableArray.IsValidIndex(i) && ScoreCollectableArray[i])
			{

				const FActorTransforms& TransformData = LoadData->ColActorPosRot[i];
				ScoreCollectableArray[i]->SetActorLocation(TransformData.ActorPosition);
				ScoreCollectableArray[i]->SetActorRotation(TransformData.ActorRotation);

			}
		}

		for (int32 i = 0; i < LoadData->HazActorPosRot.Num(); ++i)
		{

			if (HazardArray.IsValidIndex(i) && HazardArray[i])
			{

				const FActorTransforms& TransformData = LoadData->HazActorPosRot[i];
				HazardArray[i]->SetActorLocation(TransformData.ActorPosition);
				HazardArray[i]->SetActorRotation(TransformData.ActorRotation);

			}

		}

		AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MainCharacter)
		{

			const FMoveableActorTransforms& TransformData = LoadData->PlayerOneData;

			MainCharacter->SetHealth(TransformData.Health);
			MainCharacter->SetScore(TransformData.Score);
			MainCharacter->SetActorLocation(TransformData.MoveableActorPosition);
			MainCharacter->SetActorRotation(TransformData.MoveableActorRotation);

		}

		AMainCharacter* SecondCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1));
		if (SecondCharacter)
		{

			const FMoveableActorTransforms& TransformData = LoadData->PlayerTwoData;

			SecondCharacter->SetHealth(TransformData.Health);
			SecondCharacter->SetScore(TransformData.Score);
			SecondCharacter->SetActorLocation(TransformData.MoveableActorPosition);
			SecondCharacter->SetActorRotation(TransformData.MoveableActorRotation);

		}

		for (int32 i = 0; i < LoadData->EnemyPosRot.Num(); ++i)
		{

			if (EnemyArray.IsValidIndex(i) && EnemyArray[i])
			{

				const FActorTransforms& TransformData = LoadData->EnemyPosRot[i];
				EnemyArray[i]->SetActorLocation(TransformData.ActorPosition);
				EnemyArray[i]->SetActorRotation(TransformData.ActorRotation);

			}


		}

	}


}


