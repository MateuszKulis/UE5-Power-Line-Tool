#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pole.h"
#include "Components/SplineComponent.h"
#include "PowerLine.h"

#include "powerlinetoolBPLibrary.generated.h"

UCLASS()
class UpowerlinetoolBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()


public:
    UFUNCTION(BlueprintCallable, Category = "Utilities")
    static void GetPole(FString ActorPath, AActor*& OutPole,float Height, float DistanceBetweenPoles, AActor* ParentActor);

    UFUNCTION(BlueprintCallable, Category = "PowerlineTool")
    static void AddPole(AActor* PoleActor);

    UFUNCTION(BlueprintCallable, Category = "PowerlineTool")
    static void CreatePowerLine(TSubclassOf<APowerLine> LineClass,float Height, AActor* ParentActor);

    UFUNCTION(BlueprintCallable, Category = "PowerlineTool")
    static void UpdatePowerLine(float Height);

    UFUNCTION(BlueprintCallable, Category = "PowerlineTool")
    static void ClearLines();

private:
    static TArray<AActor*> PoleActors;
    static TArray<APowerLine*> PowerLineActors;
};