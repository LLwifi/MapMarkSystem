// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/WorldSubsystem/MM_Subsystem.h"
#include "MM_Config.h"
#include "Actor/MM_MarkActor.h"
#include "UserWidget/MM_Panel.h"

AMM_MarkActor* UMM_Subsystem::SpawnMarkTo(AActor* MarkOwner, AActor* ToTarget, FVector ToLocation, FMM_MarkInfo MarkInfo)
{
    AMM_MarkActor* MarkActor = GetWorld()->SpawnActor<AMM_MarkActor>(UMM_Config::GetInstance()->DefaultMarkActorClass.LoadSynchronous());
    if (MarkActor)
    {
        AllMarkActor.Add(MarkActor);

        MarkActor->SetOwner(MarkOwner);
        MarkActor->NetMultiUpdateMark(ToTarget, ToLocation, MarkInfo);
    }
    return MarkActor;
}

AMM_MarkActor* UMM_Subsystem::SpawnMarkToFromDT(AActor* MarkOwner, AActor* ToTarget, FVector ToLocation, FName RowName)
{
    UDataTable* MarkInfoDataTable = UMM_Config::GetInstance()->MarkInfoDataTable.LoadSynchronous();
    if (MarkInfoDataTable && MarkInfoDataTable->GetRowNames().Contains(RowName))
    {
        FMM_MarkInfo* MarkInfo = MarkInfoDataTable->FindRow<FMM_MarkInfo>(RowName, TEXT(""));
        return SpawnMarkTo(MarkOwner, ToTarget, ToLocation, *MarkInfo);
    }
    return nullptr;
}

void UMM_Subsystem::RemoveMark(AMM_MarkActor* MarkActor)
{
    if (MarkActor)
    {
        AllMarkActor.Remove(MarkActor);
        MarkActor->Destroy();
    }
}

UMM_Panel* UMM_Subsystem::GetMarkPanel()
{
    if (!MMPanel && !UMM_Config::GetInstance()->DefaultMarkPanelClass.IsNull())
    {
        //后面这部分应该在Panel里面统一处理
        MMPanel = Cast<UMM_Panel>(CreateWidget(GetWorld(), UMM_Config::GetInstance()->DefaultMarkPanelClass.LoadSynchronous()));
        if (MMPanel)
        {
            MMPanel->AddToViewport();
        }
    }
    return MMPanel;
}
