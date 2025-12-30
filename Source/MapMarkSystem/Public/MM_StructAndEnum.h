#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "MM_Config.h"
#include <Slate/WidgetTransform.h>
#include "MM_StructAndEnum.generated.h"

class UMM_MarkUserWidget;

//标记类型
UENUM(BlueprintType)
enum class EMM_MarkType :uint8
{
	None = 0 UMETA(DisplayName = "无")
};

/*标记射线信息
*/
USTRUCT(BlueprintType)
struct FMM_MarkRayInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceComplex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoreSelf;
};

/*屏幕标记信息
*/
USTRUCT(BlueprintType)
struct FMM_ViewMarkInfo
{
	GENERATED_BODY()
public:

	//标记是否需要在屏幕上进行追踪
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsScreenIndicators = false;

	//追踪时是否显示方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsScreenIndicators"))
	bool bIsShowDirection = false;
	//箭头图标
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsShowDirection"))
	TSoftObjectPtr<UTexture2D> ArrowTexture;
	//箭头颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsShowDirection"))
	FLinearColor ArrowColor = FLinearColor::White;
};

/*大/小地图标记信息
*/
USTRUCT(BlueprintType)
struct FMM_MapMarkInfo
{
	GENERATED_BODY()
public:
	//标记是否同步显示到小地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSyncDisplayToMiniMap = false;
	//同步到小地图的标记是否被限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToMiniMap"))
	bool bIsClampToMiniMap = false;
	//标记是否同步显示到大地图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSyncDisplayToMap = false;
};

/*水平轴标记信息
*/
USTRUCT(BlueprintType)
struct FMM_HorizontalAxisMarkInfo
{
	GENERATED_BODY()
public:
	//标记是否同步显示到水平坐标轴
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSyncDisplayToHorizontalAxis = false;

	//坐标轴标记的UI类 该标记显示在坐标轴的样式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToHorizontalAxis"))
	TSoftClassPtr<UMM_MarkUserWidget> HorizontalAxisMarkUIClass = UMM_Config::GetInstance()->DefaultHAxisMarkWidgetClass;
	//标记图标
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToHorizontalAxis"))
	TSoftObjectPtr<UTexture2D> HAxisMarkTexture;
	//标记图标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToHorizontalAxis"))
	FLinearColor HAxisMarkColor = FLinearColor::White;


	//同步到水平坐标轴的标记是否被限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToHorizontalAxis"))
	bool bIsClampToHorizontalAxis = false;
	//是否显示距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditConditionHides, EditCondition = "bIsSyncDisplayToHorizontalAxis"))
	bool bIsShowDistance = false;
};

/*标记信息
*/
USTRUCT(BlueprintType)
struct FMM_MarkInfo : public FTableRowBase
{
	GENERATED_BODY()
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		FMM_MarkInfo* MarkInfo = InDataTable->FindRow<FMM_MarkInfo>(InRowName, TEXT(""));
		if (MarkInfo)
		{
			MarkInfo->MarkID = InRowName;
		}
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MarkID;

	/*标记的UI类
	* 在普通标记和屏幕追踪时会显示该UI
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UMM_MarkUserWidget> ViewMarkUIClass = UMM_Config::GetInstance()->DefaultMarkWidgetClass;

	//标记图标
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> MarkTexture;
	//标记颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor MarkColor = FLinearColor::White;
	/*签名颜色
	* 该值的结果在UMM_Config-》RoleSignColor进行配置
	* 运行时在UMM_MarkComponent下获取RoleSign然后读取UMM_Config获取最终结果
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor RoleSignColor = FLinearColor::White;
	//标记的MarkTransform
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWidgetTransform MarkTransform;

	/*自动隐藏时间：当标记显示达到多少时间后自动隐藏
	* 该值 = -1时，表示不会因为时间而自动隐藏
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "s"))
	float AutoHideTime = -1.0f;

	/*隐藏距离：当标记与标记发起者的距离超过该值时，隐藏该标记
	* 该值 = -1时，表示不会因为距离而隐藏
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "mm"))
	float HideDistance = -1.0f;

	//是否显示距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsShowDistance = false;

	//屏幕标记信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMM_ViewMarkInfo ViewMarkInfo;
	//大/小地图标记信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMM_MapMarkInfo MapMarkInfo;
	//水平轴标记信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMM_HorizontalAxisMarkInfo HorizontalAxisMarkInfo;
};

