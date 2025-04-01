#pragma once

#include <QString>
#include <memory>
#include "core/Theme.h"

struct PlatformHandle {
    enum Type { X11, Wayland } type;

    union {
        struct {
            Display* x_display;
            Window x_window;
        };
        struct {
            wl_display* wl_display;
            wl_surface* wl_surface;
        };
    };
};


class VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    void initialize(void* nativeWindowHandle);  // Qt will provide platform-specific window pointer
    void shutdown();

    void beginFrame();
    void endFrame();

    void drawText(const QString& text, int x, int y);
    void applyTheme(const Theme& theme);

private:
    void* windowHandle = nullptr;
    Theme currentTheme;

    uint32_t currentImageIndex = 0;

    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    VkRenderPass renderPass;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    VkCommandPool commandPool;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;


    // Internal Vulkan handles (VkInstance, VkDevice, etc.)
    void createInstance();
    void selectPhysicalDevice();
    void createLogicalDevice();
    void createSwapchain();
    void createRenderPass();
    void createFramebuffers();
    void createCommandBuffers();
    void createCommandPool();
    void createSyncObjects();
    void createPipeline();
    void cleanup();

    // Add more Vulkan objects as needed
};
