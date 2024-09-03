#include "Pole.h"

APole::APole()
{
    PrimaryActorTick.bCanEverTick = true;

    PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoleMesh"));
    RootComponent = PoleMesh;
}

void APole::BeginPlay()
{
    Super::BeginPlay();

}

void APole::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APole::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    if (PropertyName == FName("PoleMesh"))
    {
        if (PoleMesh && PoleMesh->GetStaticMesh())
        {
            for (int32 MaterialIndex = 0; MaterialIndex < PoleMesh->GetNumMaterials(); ++MaterialIndex)
            {
                UMaterialInterface* Material = PoleMesh->GetMaterial(MaterialIndex);
                if (Material)
                {
                    TArray<UTexture*> Textures;
                    Material->GetUsedTextures(Textures, EMaterialQualityLevel::High, false, ERHIFeatureLevel::Num, true);
                    PoleTextures.Append(Textures);
                }
            }
        }
    }
}