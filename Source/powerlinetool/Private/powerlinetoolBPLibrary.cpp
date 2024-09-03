#include "powerlinetoolBPLibrary.h"
#include "powerlinetool.h"
#include "Editor.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "LevelInstance/LevelInstanceActor.h" 
#include "WorldPartition/WorldPartitionSubsystem.h"

UpowerlinetoolBPLibrary::UpowerlinetoolBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

TArray<AActor*> UpowerlinetoolBPLibrary::PoleActors;
TArray<APowerLine*> UpowerlinetoolBPLibrary::PowerLineActors;

void UpowerlinetoolBPLibrary::GetPole(FString ActorPath, AActor*& OutPole, float Height, float DistanceBetweenPoles, AActor* ParentActor)
{
    if (ActorPath.IsEmpty())
    {
        OutPole = nullptr;
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        OutPole = nullptr;
        return;
    }

    UBlueprint* LoadedBlueprint = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *ActorPath));
    if (!LoadedBlueprint || !LoadedBlueprint->IsA<UBlueprint>())
    {
        OutPole = nullptr;
        return;
    }

    UClass* SpawnClass = LoadedBlueprint->GeneratedClass;
    if (!SpawnClass)
    {
        OutPole = nullptr;
        return;
    }

    FVector SpawnLocation = FVector::ZeroVector;

    if (OutPole)
    {
        FVector NewLocation = OutPole->GetActorLocation() + FVector(DistanceBetweenPoles, 0.0f, 0.0f);
        NewLocation.Z = Height;
        SpawnLocation = NewLocation;
    }
    else
    {
        SpawnLocation.Z = Height;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = ParentActor;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* SpawnedActor = World->SpawnActor(SpawnClass, &SpawnLocation, nullptr, SpawnParams);
    OutPole = SpawnedActor;

    if (ParentActor && OutPole)
    {
        OutPole->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
    }
}

void UpowerlinetoolBPLibrary::AddPole(AActor* PoleActor)
{
    if (PoleActor)
    {
        PoleActors.Add(PoleActor);
    }
}

void UpowerlinetoolBPLibrary::CreatePowerLine(TSubclassOf<APowerLine> LineClass, float Height, AActor* ParentActor)
{
    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World || PoleActors.Num() < 2)
    {
        return;
    }

    UWorldPartitionSubsystem* WorldPartitionSubsystem = World->GetSubsystem<UWorldPartitionSubsystem>();
    if (!WorldPartitionSubsystem)
    {
        return;
    }

    for (int32 i = 0; i < PoleActors.Num() - 1; ++i)
    {
        AActor* StartPole = PoleActors[i];
        AActor* EndPole = PoleActors[i + 1];

        if (!StartPole || !EndPole)
        {
            continue;
        }

        FVector LineStart = StartPole->GetActorLocation();
        FVector LineEnd = EndPole->GetActorLocation();

        FVector LineLocation = (LineStart + LineEnd) / 2.0f;
        LineLocation.Z = Height;

        FRotator LineRotation = (LineEnd - LineStart).Rotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = ParentActor;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        APowerLine* LineActor = World->SpawnActor<APowerLine>(LineClass, LineLocation, LineRotation, SpawnParams);
        if (LineActor)
        {
            LineActor->StartPole = StartPole;
            LineActor->EndPole = EndPole;

            float LineLength = (LineEnd - LineStart).Size();
            LineActor->SetActorScale3D(FVector(LineLength / 2.0f, 1.0f, 1.0f));

            PowerLineActors.Add(LineActor);

            if (ParentActor && LineActor)
            {
                LineActor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepWorldTransform);
            }
        }
    }

    PoleActors.Empty();
}

void UpowerlinetoolBPLibrary::UpdatePowerLine(float Height)
{
    for (APowerLine* LineActor : PowerLineActors)
    {
        if (!LineActor)
        {
            continue;
        }

        if (LineActor->StartPole && LineActor->EndPole)
        {
            FVector LineStart = LineActor->StartPole->GetActorLocation();
            FVector LineEnd = LineActor->EndPole->GetActorLocation();

            FVector LineLocation = (LineStart + LineEnd) / 2.0f;
            LineLocation.Z = Height;

            FRotator LineRotation = (LineEnd - LineStart).Rotation();

            LineActor->SetActorLocationAndRotation(LineLocation, LineRotation);

            float LineLength = (LineEnd - LineStart).Size();
            LineActor->SetActorScale3D(FVector(LineLength / 2.0f, 1.0f, 1.0f));
        }
    }
}

void UpowerlinetoolBPLibrary::ClearLines()
{
    for (APowerLine* LineActor : PowerLineActors)
    {
        if (LineActor)
        {
            LineActor->Destroy();
        }
    }

    for (AActor* PoleActor : PoleActors)
    {
        if (PoleActor)
        {
            PoleActor->Destroy();
        }
    }

    PowerLineActors.Empty();
    PoleActors.Empty();
}
