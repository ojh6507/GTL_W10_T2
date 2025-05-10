﻿#include "UnrealEd.h"
#include "EditorPanel.h"

#include "PropertyEditor/ControlEditorPanel.h"
#include "PropertyEditor/OutlinerEditorPanel.h"
#include "PropertyEditor/PropertyEditorPanel.h"
#include "PropertyEditor/Sub/SkeletalViewerPanel.h"

void UnrealEd::Initialize()
{
    auto ControlPanel = std::make_shared<ControlEditorPanel>();
    AddEditorPanel("ControlPanel", ControlPanel);
    
    auto OutlinerPanel = std::make_shared<OutlinerEditorPanel>();
    AddEditorPanel("OutlinerPanel", OutlinerPanel);
    
    auto PropertyPanel = std::make_shared<PropertyEditorPanel>();
    AddEditorPanel("PropertyPanel", PropertyPanel);
    
    auto SubSkeletalViewerPanel = std::make_shared<SkeletalViewerPanel>();
    AddEditorPanel("SubSkeletalViewerPanel", SubSkeletalViewerPanel, true);
}

void UnrealEd::Render() const
{
    for (const auto& Panel : Panels)
    {
        Panel.Value->Render();
    }
}

void UnrealEd::RenderSkeletalSubWindowPanel() const
{
    for (const auto& Panel : SkeletalSubPanels)
    {
        Panel.Value->Render();
    }
}

void UnrealEd::AddEditorPanel(const FString& PanelId, const std::shared_ptr<UEditorPanel>& EditorPanel, bool bSubWindow)
{
    (bSubWindow ? SkeletalSubPanels : Panels)[PanelId] = EditorPanel;
}

void UnrealEd::OnResize(HWND hWnd, bool bSubWindow) const
{
    auto& targetPanels = bSubWindow ? SkeletalSubPanels : Panels;

    for (auto& PanelPair : targetPanels)
    {
        if (PanelPair.Value)
        {
            PanelPair.Value->OnResize(hWnd);
        }
    }
}

std::shared_ptr<UEditorPanel> UnrealEd::GetEditorPanel(const FString& PanelId)
{
    return Panels[PanelId];
}

std::shared_ptr<UEditorPanel> UnrealEd::GetSubEditorPanel(const FString& PanelId)
{
    return SkeletalSubPanels[PanelId];
}
