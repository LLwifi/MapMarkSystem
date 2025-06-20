// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/MM_MarkComponent.h"
#include "Subsystem/WorldSubsystem/MM_Subsystem.h"
#include "Actor/MM_MarkActor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UMM_MarkComponent::UMM_MarkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UMM_MarkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UMM_MarkComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMM_MarkComponent, MaxMarkNum);
	DOREPLIFETIME(UMM_MarkComponent, AllMarkActor);
}


// Called every frame
void UMM_MarkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AMM_MarkActor* UMM_MarkComponent::ServerCameraTraceMarkByChannel(FMM_MarkRayInfo MarkRayInfo, FHitResult& OutHit, FMM_MarkInfo MarkInfo, bool bIsAutoGetMarkInfo/* = true*/)
{
	FVector CameraLocation, TraceEndLocation;
	FRotator CameraRotation;
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn)
	{
		Pawn->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
		TraceEndLocation = (UKismetMathLibrary::GetForwardVector(CameraRotation) * TraceDistance) + CameraLocation;

		FVector MarkToLocation;
		UDataTable* MarkInfoDataTable = UMM_Config::GetInstance()->MarkInfoDataTable.LoadSynchronous();
		if (bIsAutoGetMarkInfo && UMM_Config::GetInstance()->MarkTraceInfo.Contains(nullptr))//如果是自动获取MarkInfo先将信息设置为Null的 后续如果命中了单位再尝试寻找
		{
			if (MarkInfoDataTable && MarkInfoDataTable->GetRowNames().Contains(UMM_Config::GetInstance()->MarkTraceInfo[nullptr].RowName))
			{
				MarkInfo = *MarkInfoDataTable->FindRow<FMM_MarkInfo>(UMM_Config::GetInstance()->MarkTraceInfo[nullptr].RowName, TEXT(""));
			}
		}

		//如果考虑模糊标记，这里可以使用圆形的射线
		if (UKismetSystemLibrary::LineTraceSingle(this, CameraLocation, TraceEndLocation, TraceChannel, MarkRayInfo.bTraceComplex, MarkRayInfo.ActorsToIgnore, DrawDebugType, OutHit, MarkRayInfo.bIgnoreSelf, TraceColor, TraceHitColor, DrawTime))
		{
			MarkToLocation = OutHit.Location;
			if (bIsAutoGetMarkInfo && MarkInfoDataTable && OutHit.GetActor())
			{
				for (TPair<TSoftClassPtr<AActor>, FMM_MarkInfoHandle>& pair : UMM_Config::GetInstance()->MarkTraceInfo)//根据命中的类型决定要使用的Info
				{
					if (pair.Key && MarkInfoDataTable->GetRowNames().Contains(pair.Value.RowName))
					{
						UClass* CheckClass = pair.Key.LoadSynchronous();
						if (OutHit.GetActor()->IsA(CheckClass))
						{
							MarkInfo = *MarkInfoDataTable->FindRow<FMM_MarkInfo>(pair.Value.RowName, TEXT(""));
						}

						if (CheckClass->IsChildOf(OutHit.GetActor()->GetClass()))
						{
							MarkInfo = *MarkInfoDataTable->FindRow<FMM_MarkInfo>(pair.Value.RowName, TEXT(""));
						}
					}
				}
			}
		}
		else
		{
			MarkToLocation = OutHit.TraceEnd;
		}
		return ServerMarkTo(OutHit.GetActor(), MarkToLocation, MarkInfo);
	}
	return nullptr;
}

AMM_MarkActor* UMM_MarkComponent::ServerMarkTo(AActor* ToTarget, FVector ToLocation, FMM_MarkInfo MarkInfo)
{
	//如果需要同步的话：这部分需要在服务器进行
	AMM_MarkActor* MarkActor = nullptr;

	if (AllMarkActor.Num() >= MaxMarkNum)//最大标记数量判断
	{
		MarkActor = AllMarkActor[0];
		//改变在数组的位置
		AllMarkActor.Remove(MarkActor);
		MarkActor->NetMultiUpdateMark(ToTarget, ToLocation, MarkInfo);
		AllMarkActor.Add(MarkActor);
		//一番操作后数组第一个跑到了数组最后一个位置
	}
	else
	{
		UMM_Subsystem* MySubsystem = GetWorld()->GetSubsystem<UMM_Subsystem>();
		if (MySubsystem)
		{
			MarkActor = MySubsystem->SpawnMarkTo(GetOwner(), ToTarget, ToLocation, MarkInfo);
			AllMarkActor.Add(MarkActor);
		}
	}
	return MarkActor;
}

void UMM_MarkComponent::RemoveMark(AMM_MarkActor* MarkActor)
{
	if (MarkActor)
	{
		AllMarkActor.Remove(MarkActor);
		UMM_Subsystem* MySubsystem = GetWorld()->GetSubsystem<UMM_Subsystem>();
		if (MySubsystem)
		{
			MySubsystem->RemoveMark(MarkActor);
		}
	}
}

