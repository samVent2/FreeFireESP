//
// Free Fire ESP Menu with iOS Style GUI
// Created for Free Fire v1.118.1
//

#ifndef FREEFIRE_ESP_MENU_H
#define FREEFIRE_ESP_MENU_H

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "Includes/ESP.h"
#include <cstdlib>
#include <ctime>

// Menu Configuration
namespace MenuConfig {
    // ESP Settings
    static bool espEnabled = false;
    static bool showBoxes = true;
    static bool showLines = true;
    static bool showDistance = true;
    static bool showNames = true;
    static bool showHealth = true;
    static float maxDistance = 300.0f;
    
    // Colors (iOS style)
    static float boxColor[4] = {0.0f, 0.8f, 1.0f, 1.0f};  // Cyan
    static float lineColor[4] = {1.0f, 0.3f, 0.3f, 1.0f}; // Red
    static float textColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // White
    
    // Aimbot Settings
    static bool aimbotEnabled = false;
    static bool aimAssistMode = true;
    static float aimbotFOV = 120.0f;
    static float aimbotSmoothing = 8.0f;
    static float headshotChance = 70.0f;
    static bool humanizeAim = true;
    static int reactionTime = 150;
    
    // Anti-Ban Settings
    static bool antiBanEnabled = true;
    static bool randomizeValues = true;
    static int updateDelay = 50; // milliseconds
    
    // Menu State
    static bool menuVisible = true;
    static bool firstTime = true;
}

// iOS Style Colors
namespace iOSColors {
    const ImVec4 background = ImVec4(0.11f, 0.11f, 0.12f, 0.95f);
    const ImVec4 header = ImVec4(0.15f, 0.15f, 0.16f, 1.0f);
    const ImVec4 active = ImVec4(0.0f, 0.48f, 1.0f, 1.0f);  // iOS Blue
    const ImVec4 hovered = ImVec4(0.0f, 0.48f, 1.0f, 0.8f);
    const ImVec4 text = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    const ImVec4 textDim = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    const ImVec4 border = ImVec4(0.2f, 0.2f, 0.21f, 1.0f);
    const ImVec4 separator = ImVec4(0.25f, 0.25f, 0.26f, 1.0f);
}

// Apply iOS Style to ImGui
void ApplyiOSStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Rounding (iOS style rounded corners)
    style.WindowRounding = 12.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 8.0f;
    
    // Padding and spacing
    style.WindowPadding = ImVec2(16, 16);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 20.0f;
    
    // Borders
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    
    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = iOSColors::background;
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.16f, 0.9f);
    colors[ImGuiCol_PopupBg] = iOSColors::background;
    colors[ImGuiCol_Border] = iOSColors::border;
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.21f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.26f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.31f, 1.0f);
    colors[ImGuiCol_TitleBg] = iOSColors::header;
    colors[ImGuiCol_TitleBgActive] = iOSColors::header;
    colors[ImGuiCol_TitleBgCollapsed] = iOSColors::header;
    colors[ImGuiCol_MenuBarBg] = iOSColors::header;
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.16f, 0.5f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.41f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.5f, 0.5f, 0.51f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6f, 0.6f, 0.61f, 1.0f);
    colors[ImGuiCol_CheckMark] = iOSColors::active;
    colors[ImGuiCol_SliderGrab] = iOSColors::active;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.58f, 1.0f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.0f, 0.48f, 1.0f, 0.8f);
    colors[ImGuiCol_ButtonHovered] = iOSColors::hovered;
    colors[ImGuiCol_ButtonActive] = iOSColors::active;
    colors[ImGuiCol_Header] = ImVec4(0.0f, 0.48f, 1.0f, 0.3f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.48f, 1.0f, 0.5f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.48f, 1.0f, 0.7f);
    colors[ImGuiCol_Separator] = iOSColors::separator;
    colors[ImGuiCol_SeparatorHovered] = iOSColors::active;
    colors[ImGuiCol_SeparatorActive] = iOSColors::active;
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.48f, 1.0f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.48f, 1.0f, 0.5f);
    colors[ImGuiCol_ResizeGripActive] = iOSColors::active;
    colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.21f, 1.0f);
    colors[ImGuiCol_TabHovered] = iOSColors::hovered;
    colors[ImGuiCol_TabActive] = iOSColors::active;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.16f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.21f, 1.0f);
    colors[ImGuiCol_Text] = iOSColors::text;
    colors[ImGuiCol_TextDisabled] = iOSColors::textDim;
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.48f, 1.0f, 0.35f);
}

// Anti-Ban: Add random delay
void RandomDelay() {
    if (MenuConfig::antiBanEnabled && MenuConfig::randomizeValues) {
        int delay = MenuConfig::updateDelay + (rand() % 50);
        usleep(delay * 1000);
    }
}

// Anti-Ban: Randomize float value slightly
float RandomizeFloat(float value, float variance = 0.05f) {
    if (!MenuConfig::randomizeValues) return value;
    float random = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f; // -1 to 1
    return value + (value * random * variance);
}

// Custom iOS-style Toggle Switch
bool iOSToggle(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float height = ImGui::GetFrameHeight();
    const float width = height * 1.8f;
    const float radius = height * 0.5f;

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, ImVec2(pos.x + width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + height));

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    const ImRect frame_bb(pos, ImVec2(pos.x + width, pos.y + height));

    // Animation
    const float t = *v ? 1.0f : 0.0f;
    
    // Background
    const ImU32 col_bg = *v ? 
        ImGui::GetColorU32(iOSColors::active) : 
        ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.31f, 1.0f));
    
    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, col_bg, radius);

    // Circle (knob)
    const float circle_x = frame_bb.Min.x + radius + t * (width - radius * 2.0f);
    const float circle_y = frame_bb.Min.y + radius;
    window->DrawList->AddCircleFilled(ImVec2(circle_x, circle_y), radius * 0.8f, IM_COL32(255, 255, 255, 255));

    // Label
    if (label_size.x > 0.0f)
        ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return pressed;
}

// Section Header
void SectionHeader(const char* text) {
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, iOSColors::textDim);
    ImGui::Text("%s", text);
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
}

// Main Menu Drawing Function
void DrawESPMenu() {
    if (MenuConfig::firstTime) {
        ApplyiOSStyle();
        srand(time(NULL));
        MenuConfig::firstTime = false;
    }

    if (!MenuConfig::menuVisible) return;

    ImGui::SetNextWindowSize(ImVec2(420, 750), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);

    ImGui::Begin("Free Fire ESP v1.118.1", &MenuConfig::menuVisible, 
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // Header
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(iOSColors::active, "Free Fire ESP");
    ImGui::PopFont();
    ImGui::TextColored(iOSColors::textDim, "Version 1.118.1");
    ImGui::Separator();
    ImGui::Spacing();

    // Main ESP Toggle
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
    if (iOSToggle("Enable ESP", &MenuConfig::espEnabled)) {
        RandomDelay();
    }
    ImGui::PopStyleVar();
    ImGui::Spacing();

    if (MenuConfig::espEnabled) {
        // ESP Features Section
        SectionHeader("ESP FEATURES");
        
        iOSToggle("Show Boxes", &MenuConfig::showBoxes);
        iOSToggle("Show Lines", &MenuConfig::showLines);
        iOSToggle("Show Distance", &MenuConfig::showDistance);
        iOSToggle("Show Names", &MenuConfig::showNames);
        iOSToggle("Show Health", &MenuConfig::showHealth);
        
        ImGui::Spacing();
        ImGui::Text("Max Distance");
        ImGui::SliderFloat("##maxdist", &MenuConfig::maxDistance, 50.0f, 500.0f, "%.0f m");
        
        // Colors Section
        SectionHeader("COLORS");
        
        ImGui::Text("Box Color");
        ImGui::ColorEdit4("##boxcolor", MenuConfig::boxColor, 
                         ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        
        ImGui::Text("Line Color");
        ImGui::ColorEdit4("##linecolor", MenuConfig::lineColor, 
                         ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    }

    // Aimbot Section
    SectionHeader("AIMBOT");
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
    if (iOSToggle("Enable Aimbot", &MenuConfig::aimbotEnabled)) {
        RandomDelay();
    }
    ImGui::PopStyleVar();
    
    if (MenuConfig::aimbotEnabled) {
        ImGui::Spacing();
        iOSToggle("Aim Assist Mode", &MenuConfig::aimAssistMode);
        ImGui::TextColored(iOSColors::textDim, 
                          MenuConfig::aimAssistMode ? 
                          "Subtle assistance (safer)" : 
                          "Full aimbot (risky)");
        
        ImGui::Spacing();
        ImGui::Text("FOV (Field of View)");
        ImGui::SliderFloat("##fov", &MenuConfig::aimbotFOV, 30.0f, 180.0f, "%.0f");
        
        ImGui::Text("Smoothing");
        ImGui::SliderFloat("##smooth", &MenuConfig::aimbotSmoothing, 1.0f, 20.0f, "%.1f");
        ImGui::TextColored(iOSColors::textDim, "Higher = slower/smoother");
        
        ImGui::Text("Headshot Chance");
        ImGui::SliderFloat("##hsChance", &MenuConfig::headshotChance, 0.0f, 100.0f, "%.0f%%");
        
        ImGui::Text("Reaction Time");
        ImGui::SliderInt("##reaction", &MenuConfig::reactionTime, 50, 500, "%d ms");
        
        iOSToggle("Humanize Aim", &MenuConfig::humanizeAim);
        ImGui::TextColored(iOSColors::textDim, "Adds natural imperfection");
    }
    
    ImGui::Spacing();
    
    // Anti-Ban Section
    SectionHeader("ANTI-BAN");
    
    iOSToggle("Anti-Ban Protection", &MenuConfig::antiBanEnabled);
    if (MenuConfig::antiBanEnabled) {
        iOSToggle("Randomize Values", &MenuConfig::randomizeValues);
        ImGui::Text("Update Delay");
        ImGui::SliderInt("##delay", &MenuConfig::updateDelay, 10, 200, "%d ms");
    }

    // Info Section
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(iOSColors::textDim, "ESP: %s", 
                      MenuConfig::espEnabled ? "Active" : "Inactive");
    ImGui::TextColored(iOSColors::textDim, "Aimbot: %s", 
                      MenuConfig::aimbotEnabled ? "Active" : "Inactive");
    ImGui::TextColored(iOSColors::textDim, "Anti-Ban: %s", 
                      MenuConfig::antiBanEnabled ? "Enabled" : "Disabled");

    ImGui::End();
}

#endif // FREEFIRE_ESP_MENU_H
