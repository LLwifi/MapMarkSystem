// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include <Engine/DataTable.h>
#include "MM_Config.generated.h"


class UMM_MarkUserWidget;

/*标记信息的句柄
* 主要是DT对应的行名称
*/
USTRUCT(BlueprintType)
struct FMM_MarkInfoHandle
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RowName;
};


/**
 * 编辑器下的通用配置
 */
UCLASS(config = MM_Config, defaultconfig)
class MAPMARKSYSTEM_API UMM_Config : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const final override
	{
		return FName("GameEditorConfig");
	}
	static UMM_Config* GetInstance(){ return GetMutableDefault<UMM_Config>(); }
	UFUNCTION(BlueprintPure, BlueprintCallable)
		static UMM_Config* GetMM_Config() { return GetInstance(); }

public:
	//标记信息的数据表
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> MarkInfoDataTable;

	//默认的标点UIClass
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UMM_MarkUserWidget> DefaultMarkWidgetClass;

	//默认的水平轴坐标标点UIClass
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UMM_MarkUserWidget> DefaultHAxisMarkWidgetClass;

	//标点面板的UI类
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UMM_Panel> DefaultMarkPanelClass;

	//标点的Actor类
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class AMM_MarkActor> DefaultMarkActorClass;

	//标点自动隐藏距离检测的时间间隔
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	float MarkHideCheckInterval = 1.0f;

	//标点屏幕指示器的刷新时间间隔
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	float MarkScreenIndicatorsRefreshInterval = 0.01f;

	//标点水平坐标轴的刷新时间间隔
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	float MarkHorizontalAxisRefreshInterval = 0.01f;

	/*标点射线命中不同的类要使用的标点信息
	* 判断顺序是从上往下
	* None：表示没有命中单位时的标点信息
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TMap<TSoftClassPtr<AActor>, FMM_MarkInfoHandle> MarkTraceInfo;

	/*从MapCameraActor中隐藏Actor的tag值
	* 场景中的ActorTag如果有这个值就会在相机拍摄的纹理上隐藏————在MapCameraActor的构造中会处理场景中全部的Actor
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	FName MapCameraActorHideActorTag = "HideActorFromMapCameraActor";
};
