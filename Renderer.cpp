//
// Created by devilox on 10/14/20.
//
//-----------------------------//
#include "Renderer.h"
//-----------------------------//
Renderer::Renderer(SDL_Window* tWindow) : mWindow(tWindow) {
    try {
        setInstanceExtensions();
    } catch (const std::runtime_error& tExcept) {
        throw;
    }
}
Renderer::~Renderer() {
    destroyAll();
}
//-----------------------------//
void Renderer::setApplicationName(const std::string& tAppName) {
    mAppName = tAppName;
}
void Renderer::setApplicationVersion(uint32_t tMajor, uint32_t tMinor, uint32_t tPatch) {
    mAppVersion = VK_MAKE_VERSION(tMajor, tMinor, tPatch);
}
void Renderer::setDebugMode(bool tState) {
    mDebug = tState;

    if (mDebug) {
        mInstanceLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        mInstanceExtensions.emplace_back("VK_EXT_debug_utils");
    }
}

void Renderer::init() {
    try {
        createInstance();
        createSurface();
        setPhysicalDevice();
        createLogicalDevice();
        createSwapchain();
        createRenderPass();
        createPipeline();
        createFrameBuffers();
        createCommandPool();

        std::vector <dVulkanMesh::Vertex> meshVertices {
                {{  -0.1f,  -0.4f,  0.0f}, {1.0f,   0.0f,   0.0f}},
                {{  -0.1f,  0.4f,   0.0f}, {0.0f,   1.0f,   0.0f}},
                {{  -0.9f,  0.4f,   0.0f}, {0.0f,   0.0f,   1.0f}},
                {{  -0.9f,  -0.4f,  0.0f}, {1.0f,   1.0f,   0.0f}}
        };

        std::vector <dVulkanMesh::Vertex> meshVertices2 {
                {{  0.9f,   -0.3f,  0.0f}, {1.0f,   0.0f,   0.0f}},
                {{  0.9f,   0.1f,   0.0f}, {0.0f,   1.0f,   0.0f}},
                {{  0.1f,   0.3f,   0.0f}, {0.0f,   0.0f,   1.0f}},
                {{  0.1f,   -0.3f,  0.0f}, {1.0f,   1.0f,   0.0f}}
        };

        std::vector <uint32_t> meshIndices {
                0, 1, 2,
                2, 3, 0
        };


        dVulkanMesh FirstMesh(mGPU, mLogicalGPU, mGraphicsQueue, mGraphicsCommandPool, meshVertices, meshIndices);
        dVulkanMesh SecondMesh(mGPU, mLogicalGPU, mGraphicsQueue, mGraphicsCommandPool, meshVertices2, meshIndices);

        mMeshList.reserve(2);

        mMeshList.emplace_back(FirstMesh);
        mMeshList.emplace_back(SecondMesh);

        mTestPlot = new dDensityPlot2D<360, 50>(-1.0f, -1.0f, 0.0f, 2.0f, 2.0f, mGPU, mLogicalGPU, mGraphicsQueue, mGraphicsCommandPool);
        mTestPlot -> addColor(9500.0, 0.0, 0.0, 0.5);
        mTestPlot -> addColor(9600, 0.0, 0.0, 1.0);
        mTestPlot -> addColor(9800, 0.0, 1.0, 1.0);
        mTestPlot -> addColor(10200, 1.0, 1.0, 0.0);
        mTestPlot -> addColor(10400, 1.0, 0.0, 0.0);
        mTestPlot -> addColor(10500, 0.5, 0.0, 0.0);

        mTestPlot -> generateBuffers();

        mPlotBuffers.resize(mSwapchainFrameBuffers.size());

        for (size_t i = 0; i < 3; i++) {
            mPlotBuffers[i] = mTestPlot -> getVertexBuffer(i);
        }

        createCommandBuffers();
        recordCommands();
        createSynchronization();
    } catch (const std::runtime_error& tExcept) {
        std::cerr << tExcept.what() << std::endl;
        destroyAll();
        throw;
    }
}
void Renderer::draw(const std::vector <std::vector <float>>& tNewData) {
    vkWaitForFences(mLogicalGPU, 1, &mDrawFences[mCurrentFrame], VK_TRUE, std::numeric_limits <uint64_t>::max());
    vkResetFences(mLogicalGPU, 1, &mDrawFences[mCurrentFrame]);

    uint32_t ImageIndex;
    vkAcquireNextImageKHR(mLogicalGPU, mSwapchain, std::numeric_limits <uint64_t>::max(), mImageAvailable[mCurrentFrame], VK_NULL_HANDLE, &ImageIndex);

//    mTestPlot -> randomize(ImageIndex);
    mTestPlot -> updateValues(tNewData, ImageIndex);

    VkPipelineStageFlags WaitStages[] {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSubmitInfo SubmitInfo {
        .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                  = nullptr,
        .waitSemaphoreCount     = 1,
        .pWaitSemaphores        = &mImageAvailable[mCurrentFrame],
        .pWaitDstStageMask      = WaitStages,
        .commandBufferCount     = 1,
        .pCommandBuffers        = &mCommandBuffers[ImageIndex],
        .signalSemaphoreCount   = 1,
        .pSignalSemaphores      = &mRenderFinished[mCurrentFrame]
    };

    if (vkQueueSubmit(mGraphicsQueue, 1, &SubmitInfo, mDrawFences[mCurrentFrame])) {
        throw std::runtime_error("Failed to submit command buffer to queue!");
    }

    VkPresentInfoKHR PresentInfo {
        .sType                  = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext                  = nullptr,
        .waitSemaphoreCount     = 1,
        .pWaitSemaphores        = &mRenderFinished[mCurrentFrame],
        .swapchainCount         = 1,
        .pSwapchains            = &mSwapchain,
        .pImageIndices          = &ImageIndex
    };

    if (vkQueuePresentKHR(mPresentationQueue, &PresentInfo)) {
        throw std::runtime_error("Failed to present image!");
    }

    mCurrentFrame = (mCurrentFrame + 1) % mMaxQueueImages;
}
//-----------------------------//
void Renderer::createInstance() {
    VkApplicationInfo ApplicationInfo {};

    ApplicationInfo.sType                       = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ApplicationInfo.pApplicationName            = mAppName.data();
    ApplicationInfo.applicationVersion          = mAppVersion;
    ApplicationInfo.pEngineName                 = "dEngine";
    ApplicationInfo.engineVersion               = VK_MAKE_VERSION(0, 1, 0);
    ApplicationInfo.apiVersion                  = VK_API_VERSION_1_1;

    //----------//

    VkInstanceCreateInfo InstanceCreateInfo {};

    InstanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceCreateInfo.pApplicationInfo         = &ApplicationInfo;
    InstanceCreateInfo.enabledLayerCount        = static_cast <uint32_t>(mInstanceLayers.size());
    InstanceCreateInfo.ppEnabledLayerNames      = mInstanceLayers.data();
    InstanceCreateInfo.enabledExtensionCount    = static_cast <uint32_t>(mInstanceExtensions.size());
    InstanceCreateInfo.ppEnabledExtensionNames  = mInstanceExtensions.data();

    //----------//

    VkResult Result = vkCreateInstance(&InstanceCreateInfo, nullptr, &mInstance);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error("Instance creation failed!");
    }
}
void Renderer::createSurface() {
    if (!SDL_Vulkan_CreateSurface(mWindow, mInstance, &mSurface)) {
        throw std::runtime_error("Surface creation error!");
    }
}
void Renderer::setPhysicalDevice() {
    uint32_t Count = 0;
    std::vector <VkPhysicalDevice> Devices;

    vkEnumeratePhysicalDevices(mInstance, &Count, nullptr);

    if (!Count) {
        throw std::runtime_error("No devices support Vulkan!");
    }

    Devices.resize(Count);
    vkEnumeratePhysicalDevices(mInstance, &Count, Devices.data());

    VkPhysicalDeviceProperties Prop {};

    for (const auto& iDevice : Devices) {
        vkGetPhysicalDeviceProperties(iDevice, &Prop);
        std::cout << Prop.deviceName << std::endl;

        if (    getGraphicsQueueFamilyIndex(iDevice) != -1                          &&
                getPresentationQueueFamilyIndex(iDevice, mSurface) != -1    &&
                checkDeviceExtensions(iDevice)) {
            mGPU = iDevice;
            break;
        }
    }
}
void Renderer::createLogicalDevice() {
    float Priority = 1.0f;
    int32_t GraphicsFamilyIndex = getGraphicsQueueFamilyIndex(mGPU);
    int32_t PresentationFamilyIndex = getPresentationQueueFamilyIndex(mGPU, mSurface);
    std::set <int32_t> FamilyIndices = { GraphicsFamilyIndex, PresentationFamilyIndex };

    for (const auto& iIndex : FamilyIndices) {
        if (iIndex < 0) {
            throw std::runtime_error("Invalid queue!");
        }
    }

    std::vector <VkDeviceQueueCreateInfo> DeviceQueueCreateInfoVec;

    for (const auto& iIndex : FamilyIndices) {
        VkDeviceQueueCreateInfo DeviceQueueCreateInfo {};

        DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        DeviceQueueCreateInfo.queueFamilyIndex = static_cast <uint32_t>(iIndex);
        DeviceQueueCreateInfo.queueCount = 1;
        DeviceQueueCreateInfo.pQueuePriorities = &Priority;

        DeviceQueueCreateInfoVec.emplace_back(DeviceQueueCreateInfo);
    }

    VkDeviceCreateInfo DeviceCreateInfo {};

    DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceCreateInfo.queueCreateInfoCount = static_cast <uint32_t>(DeviceQueueCreateInfoVec.size());
    DeviceCreateInfo.pQueueCreateInfos = DeviceQueueCreateInfoVec.data();
    DeviceCreateInfo.enabledExtensionCount = static_cast <uint32_t>(mDeviceExtensions.size());
    DeviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

    VkResult Result = vkCreateDevice(mGPU, &DeviceCreateInfo, nullptr, &mLogicalGPU);

    if (Result != VK_SUCCESS) {
        throw std::runtime_error("Logical device creation error!");
    }

    vkGetDeviceQueue(mLogicalGPU, static_cast <uint32_t>(GraphicsFamilyIndex), 0, &mGraphicsQueue);
    vkGetDeviceQueue(mLogicalGPU, static_cast <uint32_t>(PresentationFamilyIndex), 0, &mPresentationQueue);

    mGraphicsFamilyIndex = GraphicsFamilyIndex;
    mPresentationFamilyIndex = PresentationFamilyIndex;
}
void Renderer::createSwapchain() {
    uint32_t ImageCount;
    VkFormat Format = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR ColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    VkExtent2D Extent;
    VkPresentModeKHR PresentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;

    //----------//

    VkSurfaceCapabilitiesKHR SurfaceCapabilities {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mGPU, mSurface, &SurfaceCapabilities);

    ImageCount = SurfaceCapabilities.minImageCount + 1;

    if (SurfaceCapabilities.maxImageCount > 0) {
        ImageCount = std::min(ImageCount, SurfaceCapabilities.maxImageCount);
    }

    if (    SurfaceCapabilities.currentExtent.width != std::numeric_limits <uint32_t>::max()    &&
            SurfaceCapabilities.currentExtent.height != std::numeric_limits <uint32_t>::max()) {
        Extent = SurfaceCapabilities.currentExtent;
    } else {
        int Width;
        int Height;

        SDL_Vulkan_GetDrawableSize(mWindow, &Width, &Height);

        Extent.width = static_cast <uint32_t>(Width);
        Extent.height = static_cast <uint32_t>(Height);

        Extent.width = std::max(
                std::min(SurfaceCapabilities.currentExtent.width, SurfaceCapabilities.maxImageExtent.width),
                SurfaceCapabilities.minImageExtent.width);
        Extent.height = std::max(
                std::min(SurfaceCapabilities.currentExtent.height, SurfaceCapabilities.maxImageExtent.height),
                SurfaceCapabilities.minImageExtent.height);
    }

    //----------//

    uint32_t FormatCount;

    vkGetPhysicalDeviceSurfaceFormatsKHR(mGPU, mSurface, &FormatCount, nullptr);
    std::vector <VkSurfaceFormatKHR> SurfaceFormats(FormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(mGPU, mSurface, &FormatCount, SurfaceFormats.data());

    if (FormatCount == 1 && SurfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
        Format = VK_FORMAT_R8G8B8A8_UNORM;
    } else {
        for (const auto& iFormat : SurfaceFormats) {
            if (iFormat.format == VK_FORMAT_R8G8B8A8_UNORM) {
                Format = VK_FORMAT_R8G8B8A8_UNORM;
                break;
            }
        }

        if (Format == VK_FORMAT_UNDEFINED) {
            for (const auto& iFormat : SurfaceFormats) {
                if (iFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
                    Format = VK_FORMAT_B8G8R8A8_UNORM;
                    break;
                }
            }
        }
    }

    if (Format == VK_FORMAT_UNDEFINED) {
        throw std::runtime_error("No suitable format was found!");
    }

    //----------//

    uint32_t PresentCount;

    vkGetPhysicalDeviceSurfacePresentModesKHR(mGPU, mSurface, &PresentCount, nullptr);
    std::vector <VkPresentModeKHR> PresentModes(PresentCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(mGPU, mSurface, &PresentCount, PresentModes.data());

    for (const auto& iMode : PresentModes) {
        if (iMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            PresentMode = iMode;
            break;
        }
    }

    if (PresentMode == VK_PRESENT_MODE_MAX_ENUM_KHR) {
        PresentMode = VK_PRESENT_MODE_FIFO_KHR;
    }

    //----------//

    VkSwapchainCreateInfoKHR SwapchainCreateInfo {};

    SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    SwapchainCreateInfo.surface = mSurface;
    SwapchainCreateInfo.minImageCount = ImageCount;
    SwapchainCreateInfo.imageFormat = Format;
    SwapchainCreateInfo.imageColorSpace = ColorSpace;
    SwapchainCreateInfo.imageExtent = Extent;
    SwapchainCreateInfo.imageArrayLayers = 1;
    SwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    SwapchainCreateInfo.preTransform = SurfaceCapabilities.currentTransform;
    SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapchainCreateInfo.presentMode = PresentMode;
    SwapchainCreateInfo.clipped = VK_TRUE;
    SwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (mGraphicsFamilyIndex != mPresentationFamilyIndex) {
        std::vector <uint32_t> Queues = { static_cast<uint32_t>(mGraphicsFamilyIndex), static_cast<uint32_t>(mPresentationFamilyIndex) };

        SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        SwapchainCreateInfo.queueFamilyIndexCount = 2;
        SwapchainCreateInfo.pQueueFamilyIndices = Queues.data();
    } else {
        SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        SwapchainCreateInfo.queueFamilyIndexCount = 0;
        SwapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    if (vkCreateSwapchainKHR(mLogicalGPU, &SwapchainCreateInfo, nullptr, &mSwapchain) != VK_SUCCESS) {
        throw std::runtime_error("Swapchain creation failed!");
    }

    mFormat = Format;
    mExtent = Extent;

    uint32_t SwapchainImageCount;
    vkGetSwapchainImagesKHR(mLogicalGPU, mSwapchain, &SwapchainImageCount, nullptr);
    std::vector <VkImage> SwapchainImages(SwapchainImageCount);
    vkGetSwapchainImagesKHR(mLogicalGPU, mSwapchain, &SwapchainImageCount, SwapchainImages.data());

    for (const auto& iImage : SwapchainImages) {
        mSwapchainImages.emplace_back(iImage);

        //----------//

        VkImageViewCreateInfo ImageViewCreateInfo {};

        ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ImageViewCreateInfo.image = iImage;
        ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ImageViewCreateInfo.format = Format;
        ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        ImageViewCreateInfo.subresourceRange.levelCount = 1;
        ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        ImageViewCreateInfo.subresourceRange.layerCount = 1;

        VkImageView ImageView;

        if (vkCreateImageView(mLogicalGPU, &ImageViewCreateInfo, nullptr, &ImageView) != VK_SUCCESS) {
            throw std::runtime_error("Image view creation failed!");
        }

        mSwapchainImageViews.emplace_back(ImageView);
    }
}
void Renderer::createRenderPass() {
    VkAttachmentDescription ColorAttachmentDescription {
            .flags              = 0,
            .format             = mFormat,
            .samples            = VK_SAMPLE_COUNT_1_BIT,
            .loadOp             = VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp            = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference ColorAttachmentReference {
            .attachment     = 0,
            .layout         = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription SubpassDescription {
            .flags                  = 0,
            .pipelineBindPoint      = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount   = 0,
            .pInputAttachments      = nullptr,
            .colorAttachmentCount   = 1,
            .pColorAttachments      = &ColorAttachmentReference
    };

    std::array <VkSubpassDependency, 2> SubpassDependencies {
        VkSubpassDependency {
                .srcSubpass         = VK_SUBPASS_EXTERNAL,
                .dstSubpass         = 0,
                .srcStageMask       = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                .dstStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask      = VK_ACCESS_MEMORY_READ_BIT,
                .dstAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dependencyFlags    = 0
        },
        VkSubpassDependency {
                .srcSubpass         = 0,
                .dstSubpass         = VK_SUBPASS_EXTERNAL,
                .srcStageMask       = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask       = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                .srcAccessMask      = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask      = VK_ACCESS_MEMORY_READ_BIT,
                .dependencyFlags    = 0
        }
    };

    VkRenderPassCreateInfo RenderPassCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .attachmentCount    = 1,
            .pAttachments       = &ColorAttachmentDescription,
            .subpassCount       = 1,
            .pSubpasses         = &SubpassDescription,
            .dependencyCount    = static_cast <uint32_t> (SubpassDependencies.size()),
            .pDependencies      = SubpassDependencies.data()
    };

    if (vkCreateRenderPass(mLogicalGPU, &RenderPassCreateInfo, nullptr, &mRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render pass!");
    }
}
void Renderer::createPipeline() {
    VkShaderModule VertexShader = loadShader("Shaders/vert.spv");
    VkShaderModule FragmentShader = loadShader("Shaders/frag.spv");

    VkPipelineShaderStageCreateInfo ShaderStageCreateInfo[2] = {
            {
                    .sType      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext      = nullptr,
                    .flags      = 0,
                    .stage      = VK_SHADER_STAGE_VERTEX_BIT,
                    .module     = VertexShader,
                    .pName      = "main"
            },
            {
                    .sType      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext      = nullptr,
                    .flags      = 0,
                    .stage      = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .module     = FragmentShader,
                    .pName      = "main"
            }
    };
    
    VkVertexInputBindingDescription BindingDescription {
        .binding    = 0,
        .stride     = sizeof(dVulkanMesh::Vertex),
        .inputRate  = VK_VERTEX_INPUT_RATE_VERTEX
    };
    
    std::array <VkVertexInputAttributeDescription, 2> AttributeDescriptions {
            VkVertexInputAttributeDescription {
                    .location = 0,
                    .binding    = 0,
                    .format     = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset     = static_cast <uint32_t>(offsetof(dVulkanMesh::Vertex, Positions))
            },
            VkVertexInputAttributeDescription {
                    .location   = 1,
                    .binding    = 0,
                    .format     = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset     = static_cast <uint32_t>(offsetof(dVulkanMesh::Vertex, Colors))
            }
    };

    VkPipelineVertexInputStateCreateInfo VertexInputStateCreateInfo {
            .sType                              = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext                              = nullptr,
            .flags                              = 0,
            .vertexBindingDescriptionCount      = 1,
            .pVertexBindingDescriptions         = &BindingDescription,
            .vertexAttributeDescriptionCount    = static_cast <uint32_t>(AttributeDescriptions.size()),
            .pVertexAttributeDescriptions       = AttributeDescriptions.data()
    };

    VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .topology                   = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable     = VK_FALSE
    };

    VkViewport Viewport {
            .x          = 0,
            .y          = 0,
            .width      = static_cast <float>(mExtent.width),
            .height     = static_cast <float>(mExtent.height),
            .minDepth   = 0.0f,
            .maxDepth   = 1.0f
    };

    VkRect2D Scissor{
            .offset     = {0, 0},
            .extent     = mExtent
    };

    VkPipelineViewportStateCreateInfo ViewportStateCreateInfo{
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .viewportCount  = 1,
            .pViewports     = &Viewport,
            .scissorCount   = 1,
            .pScissors      = &Scissor
    };

    VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .depthClampEnable           = VK_FALSE,
            .rasterizerDiscardEnable    = VK_FALSE,
            .polygonMode                = VK_POLYGON_MODE_FILL,
            .cullMode                   = VK_CULL_MODE_BACK_BIT,
            .frontFace                  = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable            = VK_FALSE
    };

    RasterizationStateCreateInfo.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo MultisampleStateCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .rasterizationSamples   = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable    = VK_FALSE
    };

    VkPipelineColorBlendAttachmentState ColorBlendAttachmentState {
            .blendEnable            = VK_TRUE,
            .srcColorBlendFactor    = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor    = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp           = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor    = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor    = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp           = VK_BLEND_OP_ADD,
            .colorWriteMask         =   VK_COLOR_COMPONENT_R_BIT |
                                        VK_COLOR_COMPONENT_G_BIT |
                                        VK_COLOR_COMPONENT_B_BIT |
                                        VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .logicOpEnable      = VK_FALSE,
            .logicOp            = VK_LOGIC_OP_CLEAR,
            .attachmentCount    = 1,
            .pAttachments       = &ColorBlendAttachmentState
    };

    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo {
            .sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                      = nullptr,
            .flags                      = 0,
            .setLayoutCount             = 0,
            .pSetLayouts                = nullptr,
            .pushConstantRangeCount     = 0,
            .pPushConstantRanges        = nullptr
    };

    if (vkCreatePipelineLayout(mLogicalGPU, &PipelineLayoutCreateInfo, nullptr, &mPipelineLayout)) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo {
            .sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext                  = nullptr,
            .flags                  = 0,
            .stageCount             = 2,
            .pStages                = ShaderStageCreateInfo,
            .pVertexInputState      = &VertexInputStateCreateInfo,
            .pInputAssemblyState    = &InputAssemblyStateCreateInfo,
            .pTessellationState     = nullptr,
            .pViewportState         = &ViewportStateCreateInfo,
            .pRasterizationState    = &RasterizationStateCreateInfo,
            .pMultisampleState      = &MultisampleStateCreateInfo,
            .pDepthStencilState     = nullptr,
            .pColorBlendState       = &ColorBlendStateCreateInfo,
            .pDynamicState          = nullptr,
            .layout                 = mPipelineLayout,
            .renderPass             = mRenderPass,
            .subpass                = 0,
            .basePipelineHandle     = VK_NULL_HANDLE,
            .basePipelineIndex      = -1
    };

    if (vkCreateGraphicsPipelines(mLogicalGPU, VK_NULL_HANDLE, 1, &GraphicsPipelineCreateInfo, nullptr, &mGraphicsPipeline)) {
        throw std::runtime_error("Failed to create graphics pipeline!");
    }

    //----------//

    vkDestroyShaderModule(mLogicalGPU, VertexShader, nullptr);
    vkDestroyShaderModule(mLogicalGPU, FragmentShader, nullptr);
}
void Renderer::createFrameBuffers() {
    mSwapchainFrameBuffers.resize(mSwapchainImages.size());

    for (size_t i = 0; i < mSwapchainFrameBuffers.size(); i++) {
        std::array <VkImageView, 1> Attachments {
            mSwapchainImageViews[i]
        };

        VkFramebufferCreateInfo FrameBufferCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext              = nullptr,
            .flags              = 0,
            .renderPass         = mRenderPass,
            .attachmentCount    = static_cast <uint32_t>(Attachments.size()),
            .pAttachments       = Attachments.data(),
            .width              = mExtent.width,
            .height             = mExtent.height,
            .layers             = 1
        };

        if (vkCreateFramebuffer(mLogicalGPU, &FrameBufferCreateInfo, nullptr, &mSwapchainFrameBuffers[i])) {
            throw std::runtime_error("Failed to create frame buffer!");
        }
    }
}
void Renderer::createCommandPool() {
    VkCommandPoolCreateInfo CommandPoolCreateInfo {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext              = nullptr,
        .flags              = 0,
        .queueFamilyIndex   = static_cast <uint32_t>(getGraphicsQueueFamilyIndex(mGPU))
    };

    if (vkCreateCommandPool(mLogicalGPU, &CommandPoolCreateInfo, nullptr, &mGraphicsCommandPool)) {
        throw std::runtime_error("Failed to create command pool");
    }
}
void Renderer::createCommandBuffers() {
    mCommandBuffers.resize(mSwapchainFrameBuffers.size());

    VkCommandBufferAllocateInfo CommandBufferAllocateInfo {
        .sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext                  = nullptr,
        .commandPool            = mGraphicsCommandPool,
        .level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount     = static_cast <uint32_t>(mCommandBuffers.size())
    };

    if (vkAllocateCommandBuffers(mLogicalGPU, &CommandBufferAllocateInfo, mCommandBuffers.data())) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}
void Renderer::createSynchronization() {
    mImageAvailable.resize(mMaxQueueImages);
    mRenderFinished.resize(mMaxQueueImages);
    mDrawFences.resize(mMaxQueueImages);

    VkSemaphoreCreateInfo SemaphoreCreateInfo {
        .sType  = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VkFenceCreateInfo FenceCreateInfo {
        .sType  = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext  = nullptr,
        .flags  = VK_FENCE_CREATE_SIGNALED_BIT
    };

    for (size_t i = 0; i < mMaxQueueImages; i++) {
        if (    vkCreateSemaphore(mLogicalGPU, &SemaphoreCreateInfo, nullptr, &mImageAvailable[i])      ||
                vkCreateSemaphore(mLogicalGPU, &SemaphoreCreateInfo, nullptr, &mRenderFinished[i])      ||
                vkCreateFence(mLogicalGPU, &FenceCreateInfo, nullptr, &mDrawFences[i])) {
            throw std::runtime_error("Failed to create semaphore and/or fence!");
        }
    }
}

void Renderer::recordCommands() {
    VkCommandBufferBeginInfo CommandBufferBeginInfo {
        .sType  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
    };

    VkClearValue ClearValues[] {
            {0.6f, 0.65f, 0.4f, 1.0f}
    };

    VkRenderPassBeginInfo RenderPassBeginInfo {
        .sType          = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext          = nullptr,
        .renderPass     = mRenderPass
    };

    RenderPassBeginInfo.renderArea.extent   = mExtent,
    RenderPassBeginInfo.renderArea.offset   = {0, 0};
    RenderPassBeginInfo.clearValueCount     = 1;
    RenderPassBeginInfo.pClearValues        = ClearValues;

    for (size_t i = 0; i < mCommandBuffers.size(); i++) {
        RenderPassBeginInfo.framebuffer = mSwapchainFrameBuffers[i];

        if (vkBeginCommandBuffer(mCommandBuffers[i], &CommandBufferBeginInfo)) {
            throw std::runtime_error("Failed to start recording a command buffer!");
        }

        vkCmdBeginRenderPass(mCommandBuffers[i], &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        {
            vkCmdBindPipeline(mCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

//            VkBuffer VertexBuffers[] {
//                mPlotBuffers[i]
//            };

            VkDeviceSize Offsets[] { 0 };

//            vkCmdBindVertexBuffers(mCommandBuffers[i], 0, 1, VertexBuffers, Offsets);
            vkCmdBindVertexBuffers(mCommandBuffers[i], 0, 1, mPlotBuffers[i], Offsets);
            vkCmdBindIndexBuffer(mCommandBuffers[i], *mTestPlot -> getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(mCommandBuffers[i], static_cast<uint32_t>(mTestPlot -> getIndexCount()), 1, 0, 0, 0);
        }

        vkCmdEndRenderPass(mCommandBuffers[i]);

        if (vkEndCommandBuffer(mCommandBuffers[i])) {
            throw std::runtime_error("Failed to stop recording a command buffer!");
        }
    }
}
//-----------------------------//
void Renderer::destroyAll() {
    vkDeviceWaitIdle(mLogicalGPU);

    for (auto& iMesh : mMeshList) {
        iMesh.destroyBuffers();
    }

    for (size_t i = 0; i < mMaxQueueImages; i++) {
        vkDestroySemaphore(mLogicalGPU, mRenderFinished[i], nullptr);
        vkDestroySemaphore(mLogicalGPU, mImageAvailable[i], nullptr);
        vkDestroyFence(mLogicalGPU, mDrawFences[i], nullptr);
    }

    if (mGraphicsCommandPool) {
        vkDestroyCommandPool(mLogicalGPU, mGraphicsCommandPool, nullptr);
    }

    for (auto iFrameBuffer : mSwapchainFrameBuffers) {
        vkDestroyFramebuffer(mLogicalGPU, iFrameBuffer, nullptr);
    }

    if (mGraphicsPipeline) {
        vkDestroyPipeline(mLogicalGPU, mGraphicsPipeline, nullptr);
    }

    if (mPipelineLayout) {
        vkDestroyPipelineLayout(mLogicalGPU, mPipelineLayout, nullptr);
    }

    if (mRenderPass) {
        vkDestroyRenderPass(mLogicalGPU, mRenderPass, nullptr);
    }

    for (auto& iImageView : mSwapchainImageViews) {
        vkDestroyImageView(mLogicalGPU, iImageView, nullptr);
    }

    if (mSwapchain) {
        vkDestroySwapchainKHR(mLogicalGPU, mSwapchain, nullptr);
    }

    if (mSurface) {
        vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    }

    if (mLogicalGPU) {
        vkDestroyDevice(mLogicalGPU, nullptr);
    }

    if (mInstance) {
        vkDestroyInstance(mInstance, nullptr);
    }
}
void Renderer::setInstanceExtensions() {
    uint32_t Count = 0;

    SDL_Vulkan_GetInstanceExtensions(mWindow, &Count, nullptr);
    mInstanceExtensions.resize(Count);
    SDL_Vulkan_GetInstanceExtensions(mWindow, &Count, mInstanceExtensions.data());

    //----------//

    Count = 0;
    std::vector <VkExtensionProperties> SupportedExtensions;

    vkEnumerateInstanceExtensionProperties(nullptr, &Count, nullptr);
    SupportedExtensions.resize(Count);
    vkEnumerateInstanceExtensionProperties(nullptr, &Count, SupportedExtensions.data());

    for (const auto& iNeeded : mInstanceExtensions) {
        bool Found = false;

        for (const auto& iSupported : SupportedExtensions) {
            if (!strcmp(iNeeded, iSupported.extensionName)) {
                Found = true;
                break;
            }
        }

        if (!Found) {
            throw std::runtime_error("Some instance extensions are not supported!");
        }
    }
}
bool Renderer::checkDeviceExtensions(VkPhysicalDevice tDevice) {
    mDeviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    uint32_t Count = 0;

    vkEnumerateDeviceExtensionProperties(tDevice, nullptr, &Count, nullptr);
    std::vector <VkExtensionProperties> SupportedExtensions(Count);
    vkEnumerateDeviceExtensionProperties(tDevice, nullptr, &Count, SupportedExtensions.data());

    for (const auto& iNeeded : mDeviceExtensions) {
        bool Found = false;

        for (const auto& iSupported : SupportedExtensions) {
            if (!strcmp(iNeeded, iSupported.extensionName)) {
                Found = true;
                break;
            }
        }

        if (!Found) {
            return false;
        }
    }

    return true;
}
VkShaderModule Renderer::loadShader(const std::string& tPath) {
    std::ifstream ShaderFile;
    size_t FileSize;
    char* ShaderData = nullptr;

    //----------//

    ShaderFile.open(tPath, std::ios::binary | std::ios::ate);

    if (!ShaderFile.is_open()) {
        std::cout << "No shader" << std::endl;
        //---Throw exception---//
        throw std::runtime_error("No shader");
    }

    FileSize = static_cast <size_t>(ShaderFile.tellg());
    ShaderData = new char[FileSize];

    ShaderFile.seekg(0);
    ShaderFile.read(ShaderData, FileSize);

    ShaderFile.close();

    //----------//

    VkShaderModuleCreateInfo ShaderModuleCreateInfo {};
    VkShaderModule ShaderModule;

    ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ShaderModuleCreateInfo.codeSize = FileSize;
    ShaderModuleCreateInfo.pCode = reinterpret_cast <const uint32_t*>(ShaderData);

    if (vkCreateShaderModule(mLogicalGPU, &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS) {
        std::cout << "Cannot create shader" << std::endl;
        delete[](ShaderData);

        //---Throw exception---//
        throw std::runtime_error("Cannot create shader");
    }

    //----------//

    delete[](ShaderData);

    return ShaderModule;
}
//-----------------------------//
int32_t Renderer::getGraphicsQueueFamilyIndex(VkPhysicalDevice tDevice) {
    uint32_t Count = 0;
    std::vector <VkQueueFamilyProperties> Families;
    int32_t QueueIndex = -1;

    vkGetPhysicalDeviceQueueFamilyProperties(tDevice, &Count, nullptr);
    Families.resize(Count);
    vkGetPhysicalDeviceQueueFamilyProperties(tDevice, &Count, Families.data());

    for (uint32_t i = 0; i < Count; i++) {
        if (Families[i].queueCount > 0 && Families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            QueueIndex = i;
            break;
        }
    }

    return QueueIndex;
}
int32_t Renderer::getPresentationQueueFamilyIndex(VkPhysicalDevice tDevice, VkSurfaceKHR tSurface) {
    if (!tSurface) {
        throw std::runtime_error("The surface is not defined!");
    }

    uint32_t Count = 0;
    std::vector <VkQueueFamilyProperties> Families;
    int32_t QueueIndex = -1;
    VkBool32 Support = VK_FALSE;

    vkGetPhysicalDeviceQueueFamilyProperties(tDevice, &Count, nullptr);
    Families.resize(Count);
    vkGetPhysicalDeviceQueueFamilyProperties(tDevice, &Count, Families.data());

    for (uint32_t i = 0; i < Count; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(tDevice, i, tSurface, &Support);

        if (Support) {
            QueueIndex = i;
            break;
        }
    }

    return QueueIndex;
}