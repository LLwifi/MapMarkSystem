// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyTypeCustomization/MarkInfoCustomization.h"
#include <IDetailChildrenBuilder.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Misc/MessageDialog.h>
#include <Misc/PackageName.h>
#include <FileHelpers.h>
#include <DetailWidgetRow.h>
#include "SSearchableComboBox.h"
#include <PropertyCustomizationHelpers.h>

#define LOCTEXT_NAMESPACE "MarkInfoCustomization"

void IMarkInfoCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ];
}

void IMarkInfoCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    //通过名称拿结构体变量
    RowNameHandle = PropertyHandle->GetChildHandle("RowName");

    void* ValuePtr;
    PropertyHandle->GetValueData(ValuePtr);
    if (ValuePtr != nullptr)
    {
        MarkInfoHandle = (FMM_MarkInfoHandle*)ValuePtr;
    }

    //Refresh(RowNames, ResourceNameOrIndexs);
    
    UDataTable* DT = UMM_Config::GetInstance()->MarkInfoDataTable.LoadSynchronous();
    TArray<FName> DTRowName;
    if (DT)
    {
        DTRowName = DT->GetRowNames();
        for (FName& name : DTRowName)
        {
            RowNames.Add(MakeShareable(new FString(name.ToString())));
        }
    }

    //slate
    ChildBuilder.AddCustomRow(FText())
        [
            SNew(SVerticalBox)          
            + SVerticalBox::Slot()
            [
                SAssignNew(SearchableComboBox_RowName, SSearchableComboBox)
                    .OptionsSource(&RowNames)//所有选项
                    .OnGenerateWidget(this, &IMarkInfoCustomization::OnGenerateWidget_RowName)//每个下拉选项的样式通过函数构造
                    .OnSelectionChanged(this, &IMarkInfoCustomization::OnSelectionChanged_RowName)//改变选择的回调
                    [
                        SAssignNew(ComboBox_Name_Text, STextBlock)
                            .Text(FText::FromString(MarkInfoHandle->RowName.ToString()))
                    ]
            ]
        ];
}

void IMarkInfoCustomization::OnSelectionChanged_RowName(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo)
{
    RowNameHandle->SetValue(FName(*InItem.Get()));
    ComboBox_Name_Text->SetText(FText::FromString(*InItem));
    MarkInfoHandle->RowName = FName(*InItem.Get());

    //Refresh(RowNames, ResourceNameOrIndexs);
}

TSharedRef<SWidget> IMarkInfoCustomization::OnGenerateWidget_RowName(TSharedPtr<FString> InItem)
{
    return SNew(STextBlock)
        .Text(FText::FromString(*InItem));
}

#undef LOCTEXT_NAMESPACE