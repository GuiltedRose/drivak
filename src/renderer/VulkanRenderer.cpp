#include "renderer/VulkanRenderer.h"
#include <iostream>

VulkanRenderer::VulkanRenderer() {
    std::cout << "[Vulkan] Renderer created." << std::endl;
}

void VulkanRenderer::initialize(void* nativeWindowHandle) {
    windowHandle = nativeWindowHandle;
    createVulkanInstance();
    pickPhysicalDevice();
    createLogicalDevice();
    createSurface();       // requires windowHandle
    createSwapchain();
    createRenderPass();
    createPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSyncObjects();
}

void VulkanRenderer::createInstance() {
    std::cout << "[Vulkan] Creating instance..." << std::endl; // Debug console command.

    // Application discriptors
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Drivak";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "DrivakEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2; // You can lower if needed
    
    // OS checks
    const char* extensions[] = {
        "VK_KHR_surface",
#ifdef _WIN32
        "VK_KHR_win32_surface"
#elif defined(__linux__)
        "VK_KHR_xcb_surface" // or VK_KHR_wayland_surface depending on Qt backend
#endif
    };
    // Adds our app info into our vulkan drivers.
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]);
    createInfo.ppEnabledExtensionNames = extensions;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create instance!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Instance created successfully." << std::endl;
}

void VulkanRenderer::selectPhysicalDevice() {
    std::cout << "[Vulkan] Selecting physical device..." << std::endl;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "[Vulkan] No GPUs with Vulkan support found!" << std::endl;
        std::abort();
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;

    for (const auto& device : devices) {
        // Get device properties
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        // Get queue family support
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        bool hasGraphicsQueue = false;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                hasGraphicsQueue = true;
                break;
            }
        }

        if (!hasGraphicsQueue) continue; // Skip devices that can't render

        // Prefer discrete GPU
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            physicalDevice = device;
            std::cout << "[Vulkan] Selected discrete GPU: " << props.deviceName << std::endl;
            return;
        }

        // If no discrete found, remember this as fallback
        if (bestDevice == VK_NULL_HANDLE) {
            bestDevice = device;
            std::cout << "[Vulkan] Found fallback GPU: " << props.deviceName << std::endl;
        }
    }

    // If no discrete GPU, fallback to the best valid one
    if (physicalDevice == VK_NULL_HANDLE && bestDevice != VK_NULL_HANDLE) {
        physicalDevice = bestDevice;
        std::cout << "[Vulkan] Using fallback GPU." << std::endl;
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        std::cerr << "[Vulkan] No suitable GPU found!" << std::endl;
        std::abort();
    }
}

void VulkanRenderer::createLogicalDevice() {
    std::cout << "[Vulkan] Creating logical device..." << std::endl;

    // 1. Find graphics queue family
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int graphicsFamilyIndex = -1;
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamilyIndex = static_cast<int>(i);
            break;
        }
    }

    if (graphicsFamilyIndex == -1) {
        std::cerr << "[Vulkan] Failed to find graphics queue family!" << std::endl;
        std::abort();
    }

    // 2. Create device queue
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // 3. Create logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    VkResult result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    if (result != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create logical device!" << std::endl;
        std::abort();
    }

    // 4. Retrieve the graphics queue handle
    vkGetDeviceQueue(device, graphicsFamilyIndex, 0, &graphicsQueue);

    std::cout << "[Vulkan] Logical device and graphics queue created successfully." << std::endl;
}

void VulkanRenderer::createSurface() {
    std::cout << "[Vulkan] Creating surface..." << std::endl;

#ifdef _WIN32
    HWND hwnd = static_cast<HWND>(windowHandle);
    HINSTANCE hinstance = GetModuleHandle(nullptr); // Qt doesn't always give this; fallback is okay

    VkWin32SurfaceCreateInfoKHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hwnd = hwnd;
    surfaceInfo.hinstance = hinstance;

    if (vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create Win32 surface!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Surface created successfully." << std::endl;
#endif


#ifdef __linux__
    auto* platformHandle = static_cast<PlatformHandle*>(windowHandle);

    if (platformHandle->type == PlatformHandle::Wayland) {
        VkWaylandSurfaceCreateInfoKHR surfaceInfo{};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.display = platformHandle->wl_display;
        surfaceInfo.surface = platformHandle->wl_surface;

        if (vkCreateWaylandSurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS) {
            std::cerr << "[Vulkan] Failed to create Wayland surface!" << std::endl;
            std::abort();
        }

        std::cout << "[Vulkan] Wayland surface created." << std::endl;

    } else if (platformHandle->type == PlatformHandle::X11) {
        VkXlibSurfaceCreateInfoKHR surfaceInfo{};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.dpy = platformHandle->x_display;
        surfaceInfo.window = platformHandle->x_window;

        if (vkCreateXlibSurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS) {
            std::cerr << "[Vulkan] Failed to create X11 surface!" << std::endl;
            std::abort();
        }

        std::cout << "[Vulkan] X11 surface created." << std::endl;
    } else {
        std::cerr << "[Vulkan] Unknown platform type!" << std::endl;
        std::abort();
    }
#endif

}

void VulkanRenderer::createSwapchain() {
    std::cout << "[Vulkan] Creating swapchain..." << std::endl;

    // 1. Query surface capabilities
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    // 2. Choose swap extent (use currentExtent if defined)
    VkExtent2D extent = capabilities.currentExtent;
    if (extent.width == UINT32_MAX) {
        extent.width = 800;
        extent.height = 600; // fallback size (can be dynamic later)
    }

    // 3. Choose surface format
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

    VkSurfaceFormatKHR chosenFormat = formats[0];
    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            chosenFormat = availableFormat;
            break;
        }
    }

    // 4. Choose present mode
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

    VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR; // VSync (always supported)
    for (const auto& mode : presentModes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            chosenPresentMode = mode; // Low-latency triple buffering (if available)
            break;
        }
    }

    // 5. Create the swapchain
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && createInfo.minImageCount > capabilities.maxImageCount)
        createInfo.minImageCount = capabilities.maxImageCount;
    createInfo.imageFormat = chosenFormat.format;
    createInfo.imageColorSpace = chosenFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Assume graphics queue handles presentation too (true on most systems)
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = chosenPresentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create swapchain!" << std::endl;
        std::abort();
    }

    // Get swapchain images
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    // Store format and extent for later use
    swapchainImageFormat = chosenFormat.format;
    swapchainExtent = extent;

    // Create image views
    swapchainImageViews.resize(imageCount);
    for (size_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = swapchainImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = swapchainImageFormat;
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &viewInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
            std::cerr << "[Vulkan] Failed to create image view!" << std::endl;
            std::abort();
        }
    }

    std::cout << "[Vulkan] Swapchain created successfully." << std::endl;
}

void VulkanRenderer::createRenderPass() {
    std::cout << "[Vulkan] Creating render pass..." << std::endl;

    // 1. Describe the color attachment (i.e. swapchain image)
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB; // match swapchain format
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;    // clear at start of frame
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;  // store after render (for presentation)
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // ready to present

    // 2. Reference the attachment in a subpass
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // 3. Optional subpass dependency to ensure correct ordering
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // 4. Create the render pass
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create render pass!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Render pass created successfully." << std::endl;
}

void VulkanRenderer::createFramebuffers() {
    framebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
            std::cerr << "[Vulkan] Failed to create framebuffer!" << std::endl;
            std::abort();
        }
    }

    std::cout << "[Vulkan] Framebuffers created." << std::endl;
}

void VulkanRenderer::createCommandPool() {
    std::cout << "[Vulkan] Creating command pool..." << std::endl;

    // We already know which queue family index supports graphics
    // Reuse same logic as in createLogicalDevice or store index

    uint32_t queueFamilyIndex = 0; // ← You may want to cache this in createLogicalDevice

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create command pool!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Command pool created." << std::endl;
}

void VulkanRenderer::createCommandBuffers() {
    commandBuffers.resize(framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to allocate command buffers!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Command buffers allocated." << std::endl;
}

void VulkanRenderer::beginFrame() {
    // 1. Acquire next image from swapchain
    VkResult result = vkAcquireNextImageKHR(
        device, swapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &currentImageIndex);

    if (result != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to acquire swapchain image!" << std::endl;
        std::abort();
    }

    // 2. Begin recording command buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Only used for secondary buffers

    VkCommandBuffer commandBuffer = commandBuffers[currentImageIndex];

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to begin recording command buffer!" << std::endl;
        std::abort();
    }

    // 3. Begin the render pass
    VkClearValue clearColor = { {{ 0.1f, 0.1f, 0.1f, 1.0f }} }; // Default dark gray
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffers[currentImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapchainExtent;
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    std::cout << "[Vulkan] Frame begun." << std::endl;
}

void VulkanRenderer::createSyncObjects() {
    std::cout << "[Vulkan] Creating synchronization objects..." << std::endl;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to create synchronization objects!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Sync objects created." << std::endl;
}

void VulkanRenderer::endFrame() {
    VkCommandBuffer commandBuffer = commandBuffers[currentImageIndex];

    // 1. End render pass
    vkCmdEndRenderPass(commandBuffer);

    // 2. End command buffer recording
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to record command buffer!" << std::endl;
        std::abort();
    }

    // 3. Submit command buffer
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to submit draw command buffer!" << std::endl;
        std::abort();
    }

    // 4. Present the image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapchains[] = { swapchain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = &currentImageIndex;

    VkResult result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

    if (result != VK_SUCCESS) {
        std::cerr << "[Vulkan] Failed to present swapchain image!" << std::endl;
        std::abort();
    }

    std::cout << "[Vulkan] Frame submitted and presented." << std::endl;
}

void VulkanRenderer::applyTheme(const Theme& theme) {
    currentTheme = theme;
    std::cout << "[Vulkan] Applied theme: " << theme.name << std::endl;
    // In future: update shader uniforms or descriptor sets with theme color/font data
}

void VulkanRenderer::drawText(const std::string& text, int x, int y) {
    std::cout << "[Vulkan] Drawing text: '" << text << "' at (" << x << ", " << y << ")" << std::endl;
    // Actual Vulkan text rendering would happen here
}

void VulkanRenderer::renderFrame() {
    std::cout << "[Vulkan] Rendering frame with current UI..." << std::endl;
    // Swapchain present logic goes here
}
