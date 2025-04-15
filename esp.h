#pragma once
#include <Windows.h>
#include <vulkan/vulkan.h>

extern "C" {
    void InitializeESP();
    void RunESP();
    void SetESPEnabled(bool enabled);
    void SetESPFOV(float fov);
    bool IsESPEnabled();
    void SetVulkanDevice(VkDevice device, VkCommandBuffer cmdBuffer);
    void SetViewport(float width, float height);
}
