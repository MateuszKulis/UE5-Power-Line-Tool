
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerLine.generated.h"

UCLASS()
class POWERLINETOOL_API APowerLine : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    AActor* StartPole;

    UPROPERTY(EditAnywhere)
    AActor* EndPole;

};


