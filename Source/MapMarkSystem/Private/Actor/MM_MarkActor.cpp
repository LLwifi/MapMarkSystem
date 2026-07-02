// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MM_MarkActor.h"
#include <Components/WidgetComponent.h>
#include "MM_Config.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <SceneComponent/MM_AttachComponent.h>
#include "UserWidget/MM_MarkUserWidget.h"
#include "Subsystem/WorldSubsystem/MM_Subsystem.h"
#include "UserWidget/MM_Panel.h"

AMM_MarkActor::AMM_MarkActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;//避免网络剔除
	SetReplicateMovement(true);//服务器设置标记位置时，客户端进行位置同步

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(SceneComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetWidgetClass(UMM_Config::GetInstance()->DefaultMarkWidgetClass.LoadSynchronous());
	WidgetComponent->SetDrawAtDesiredSize(true);
}

void AMM_MarkActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMM_MarkActor, bIsShow);

	DOREPLIFETIME(AMM_MarkActor, MarkTarget);
	DOREPLIFETIME(AMM_MarkActor, MarkLocation);
	DOREPLIFETIME(AMM_MarkActor, SoundBase);

	DOREPLIFETIME(AMM_MarkActor, MarkInfo);
}

void AMM_MarkActor::BeginPlay()
{
	Super::BeginPlay();

	if (!UKismetSystemLibrary::IsServer(this))//服务器不需要UI
	{
		//MarkUserWidget = Cast<UMM_MarkUserWidget>(CreateWidget(GetWorld(), UMM_Config::GetInstance()->DefaultMarkWidgetClass.LoadSynchronous()));
		MarkUserWidget = Cast<UMM_MarkUserWidget>(WidgetComponent->GetWidget());
		MarkUserWidget->MarkActor = this;
		WidgetComponent->SetWidget(MarkUserWidget);

		// 绑定关卡切换事件 在世界分区时，该回调会频繁调用
		//FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &AMM_MarkActor::OnLevelRemoved);
	}

	if (bBeginPlayIsUpdateMark)
	{
		NetMultiUpdateMark(MarkTarget, MarkLocation, MarkInfo);
	}
}

void AMM_MarkActor::Destroyed()
{
	Super::Destroyed();

}

void AMM_MarkActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (MarkUserWidget)
	{
		MarkUserWidget->RemoveFromParent();
		if (MarkUserWidget->HAxisMark)
		{
			MarkUserWidget->HAxisMark->RemoveFromParent();
		}
	}
}

void AMM_MarkActor::ServerShowMark_Implementation()
{
	if (!bIsShow)
	{
		bIsShow = true;
		//自动隐藏逻辑
		if (MarkInfo.AutoHideTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoHideTimeHandle, this, &AMM_MarkActor::ServerHideMark, MarkInfo.AutoHideTime);
		}

		//如果是纯在客户端调用该函数，那么也需要对UI进行处理
		if (!UKismetSystemLibrary::IsServer(this))
		{
			ReplicatedUsing_IsShowChange();
		}
	}
}

void AMM_MarkActor::ServerHideMark_Implementation()
{
	if (bIsShow)
	{
		bIsShow = false;

		//如果是纯在客户端调用该函数，那么也需要对UI进行处理
		if (!UKismetSystemLibrary::IsServer(this))
		{
			ReplicatedUsing_IsShowChange();
		}
	}
}

void AMM_MarkActor::NetMultiUpdateMark_Implementation(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo NewMarkInfo)
{
	MarkTarget = ToTarget;
	MarkLocation = ToLocation;
	MarkInfo = NewMarkInfo;

	if (MarkTarget)
	{
		UMM_AttachComponent* AttachComponent = MarkTarget->GetComponentByClass<UMM_AttachComponent>();
		if (AttachComponent)//只有有Attach组件的目标才算是一个“可以被追踪标记的目标” 否则这次仅认为是一个位置标记
		{
			AttachToComponent(AttachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			//GetRootComponent()->SetRelativeLocation(MarkLocation);
		}
		else
		{
			SetActorLocation(MarkLocation);
			AttachToActor(MarkTarget, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	else
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SetActorLocation(MarkLocation);
	}

	PlayMarkSound();//播放标记声音

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		ServerShowMark();

		//自动隐藏逻辑
		if (MarkInfo.AutoHideTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoHideTimeHandle, this, &AMM_MarkActor::ServerHideMark, MarkInfo.AutoHideTime);
		}
		//隐藏距离检测
		if (MarkInfo.HideDistance > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(HideDistanceCheckTimeHandle, this, &AMM_MarkActor::HideDistanceCheck, UMM_Config::GetInstance()->MarkHideCheckInterval, true);
		}
	}

	if (!UKismetSystemLibrary::IsServer(this))
	{
		if (MarkUserWidget)
		{
			MarkUserWidget->UpdateMark(ToTarget, ToLocation, MarkInfo);
		}

		UMM_Subsystem* MySubsystem = GetWorld()->GetSubsystem<UMM_Subsystem>();
		if (MySubsystem && MySubsystem->GetMarkPanel())
		{
			MySubsystem->GetMarkPanel()->RefreshMark(this);//刷新标记
		}
		//屏幕追踪时控件就不需要显示了
		WidgetComponent->SetVisibility(!MarkInfo.ViewMarkInfo.bIsScreenIndicators);
	}
}

void AMM_MarkActor::HideDistanceCheck()
{
	if (GetMarkDistance(GetOwner()->GetActorLocation()) > MarkInfo.HideDistance)
	{
		ServerHideMark();
	}
	else
	{
		ServerShowMark();
	}
}

float AMM_MarkActor::GetMarkDistance(FVector TargetLocation)
{
	return UKismetMathLibrary::VSize(TargetLocation - GetActorLocation());
}

float AMM_MarkActor::GetMarkAngle(FVector TargetLocation, FVector TargetDir)
{
	FVector MarkDir = GetActorLocation() - TargetLocation;//目标位置指向我
	float BaseAngle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Vector_CosineAngle2D(MarkDir, TargetDir));
	FVector Cross = UKismetMathLibrary::Cross_VectorVector(MarkDir, TargetDir);//判断是否越过了180度

	if (Cross.Z > 0.0f)
	{
		return 360.0f - BaseAngle;
	}
	else
	{
		return BaseAngle;
	}
}

void AMM_MarkActor::PlayMarkSound_Implementation()
{
	if (SoundBase)
	{
		UGameplayStatics::CreateSound2D(this, SoundBase);
	}
}

void AMM_MarkActor::OnLevelRemoved_Implementation(ULevel* InLevel, UWorld* InWorld)
{
	if (MarkUserWidget)
	{
		MarkUserWidget->RemoveFromParent();
	}
}

void AMM_MarkActor::ReplicatedUsing_IsShowChange()
{
	if (MarkUserWidget)
	{
		if (bIsShow)
		{
			MarkUserWidget->ShowMark();
		}
		else
		{
			MarkUserWidget->HideMark();
		}
	}
}

