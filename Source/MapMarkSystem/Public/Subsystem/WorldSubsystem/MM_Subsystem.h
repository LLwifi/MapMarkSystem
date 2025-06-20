// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MM_StructAndEnum.h"
#include "MM_Subsystem.generated.h"

class AMM_MarkActor;
class UMM_Panel;

/**
 * 标记子系统，继承自UWorldSubsystem 标记在切换地图/区域时应该被清除
 * 管理全部存在的标记
 * 用该类进行标记的创建
 */
UCLASS()
class MAPMARKSYSTEM_API UMM_Subsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	//创建标记到...（标记发起/拥有者，标记目标位置，标记单位Actor，标记信息）
	UFUNCTION(BlueprintCallable)
	AMM_MarkActor* SpawnMarkTo(AActor* MarkOwner, AActor* ToTarget, FVector ToLocation, FMM_MarkInfo MarkInfo);

	//创建标记到...信息取自DataTable表（标记发起/拥有者，标记目标位置，标记单位Actor，行名称）
	UFUNCTION(BlueprintCallable)
	AMM_MarkActor* SpawnMarkToFromDT(AActor* MarkOwner, AActor* ToTarget, FVector ToLocation, FName RowName);

	//移除标记
	UFUNCTION(BlueprintCallable)
	void RemoveMark(AMM_MarkActor* MarkActor);

	//获取标记面板
	UFUNCTION(BlueprintPure)
	UMM_Panel* GetMarkPanel();

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<AMM_MarkActor*> AllMarkActor;

	//标记面板
	UPROPERTY(BlueprintReadWrite)
	UMM_Panel* MMPanel;
};
