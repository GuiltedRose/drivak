#pragma once

#include <vulkan/vulkan.h>
#ifdef _WIN32
    // Windows-specific includes
    #include <windows.h>   // For Windows-specific APIs
#elif defined(__linux__)
    #include <X11/Xlib.h> 
    #include <wayland-client.h>
    #include <unistd.h>      // For Linux-specific system calls
#endif
#include <vector>
#include <QString>
#include <memory>
#include "core/ThemeLoader.h"

struct PlatformHandle {
    enum Type { X11, Wayland } type;
    union PlatformData {
        struct X11Data {
            Display* x_display;
            Window x_window;
        } x11;

        struct WaylandData {
            wl_display* wayland_display;  // Changed name
            wl_surface* wayland_surface;  // Changed name
        } wayland;
    } platformData;
};


struct QueuedText {
    std::string text;
    int x, y;
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
    void applyTheme(const ThemeLoader& theme);

private:
    void* windowHandle = nullptr;
    ThemeLoader currentTheme;

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
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkShaderModule createShaderModule(const std::vector<char>& code);

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<QueuedText> textQueue;
    
    std::vector<char> loadShader(const std::string& path);

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
    void recreateSwapchain();
    void cleanupSwapchain();  // helper for teardown


    // Add more Vulkan objects as needed
};
