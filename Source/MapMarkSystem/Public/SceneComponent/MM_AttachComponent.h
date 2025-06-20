// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MM_AttachComponent.generated.h"

/*标记挂点
* 当标记有目标（Actor）时，会尝试在目标身上获取该组件，并将标记Actor挂载到该组件上
* 主要用于确定标记在目标（Actor）上的位置
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAPMARKSYSTEM_API UMM_AttachComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMM_AttachComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
