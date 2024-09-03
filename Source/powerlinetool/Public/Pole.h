#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h" 
#include "Pole.generated.h"

UCLASS()
class POWERLINETOOL_API APole : public AActor
{
	GENERATED_BODY()

public:
	APole();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pole")
	UStaticMeshComponent* PoleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pole")
	TArray<UTexture*> PoleTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pole")
	FVector AttachmentPoint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pole")
	FVector AttachmentPoint2;

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	virtual void Tick(float DeltaTime) override;

};