#include "menu.h"
#include "aimbot.h"
#include "esp.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include <Windows.h>

static bool showMenu = false;
static HWND gWindow = nullptr;
static LPDIRECT3DDEVICE9 gDevice = nullptr;

void InitializeMenu(HWND window, LPDIRECT3DDEVICE9 pDevice) {
    gWindow = window;
    gDevice = pDevice;
    
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Customize colors to match the screenshot
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.25f, 0.66f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.57f, 0.35f, 0.76f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.47f, 0.25f, 0.66f, 1.00f);
}

void RenderMenu() {
    if (!showMenu) return;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Evicted Cheat Menu", &showMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    // Aimbot Section
    ImGui::Text("Aimbot");
    bool aimbotEnabled = IsAimbotEnabled();
    if (ImGui::Checkbox("Enable Aimbot", &aimbotEnabled)) {
        SetAimbotEnabled(aimbotEnabled);
    }
    
    static float aimbotFOV = 5.0f;
    if (ImGui::SliderFloat("Aimbot FOV", &aimbotFOV, 1.0f, 30.0f)) {
        SetAimbotFOV(aimbotFOV);
    }
    
    static float smoothFactor = 5.0f;
    if (ImGui::SliderFloat("Smooth Factor", &smoothFactor, 1.0f, 20.0f)) {
        SetSmoothFactor(smoothFactor);
    }

    ImGui::Separator();

    // ESP Section
    ImGui::Text("ESP");
    bool espEnabled = IsESPEnabled();
    if (ImGui::Checkbox("Enable ESP", &espEnabled)) {
        SetESPEnabled(espEnabled);
    }
    
    static float espFOV = 90.0f;
    if (ImGui::SliderFloat("ESP FOV", &espFOV, 30.0f, 180.0f)) {
        SetESPFOV(espFOV);
    }

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ToggleMenu() {
    showMenu = !showMenu;
}

bool IsMenuVisible() {
    return showMenu;
}

void CleanupMenu() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
