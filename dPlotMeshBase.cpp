//
// Created by devilox on 5/1/21.
//
//-----------------------------//
#include "dPlotMeshBase.h"
//-----------------------------//
dPlotMeshBase::dPlotMeshBase() {
    mVertexBuffers.resize(3);
    mVertexBuffersMemory.resize(3);
}
//-----------------------------//
VkBuffer* dPlotMeshBase::getVertexBuffer(size_t iNum) {
    return &mVertexBuffers[iNum];
}
VkBuffer* dPlotMeshBase::getIndexBuffer() {
    return &mIndexBuffer;
}

void dPlotMeshBase::destroyBuffers() {
    for (size_t i = 0; i < mVertexBuffers.size(); i++) {
        vkDestroyBuffer(mLogicalGPU, mVertexBuffers[i], nullptr);
        vkFreeMemory(mLogicalGPU, mVertexBuffersMemory[i], nullptr);
    }

    vkDestroyBuffer(mLogicalGPU, mIndexBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, mIndexBufferMemory, nullptr);
}
//-----------------------------//
void dPlotMeshBase::createVertexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const Vertex* tVertices, size_t tSize) {
    VkDeviceSize    BufferSize              = sizeof(Vertex) * tSize;
    VkBuffer        StagingBuffer;
    VkDeviceMemory  StagingBufferMemory;
    void*           Data;

    createBuffer(
            BufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &StagingBuffer,
            &StagingBufferMemory
    );

    vkMapMemory(mLogicalGPU, StagingBufferMemory, 0, BufferSize, 0, &Data);
    memcpy(Data, tVertices, static_cast <size_t>(BufferSize));
    vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

    for (size_t i = 0; i < mVertexBuffers.size(); i++) {
        createBuffer(
                BufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                &mVertexBuffers[i],
                &mVertexBuffersMemory[i]
        );

        copyBuffer(tTransferQueue, tTransferCommandPool, StagingBuffer, mVertexBuffers[i], BufferSize);
    }

    vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
}
void dPlotMeshBase::createIndexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const uint32_t* tIndices, size_t tSize) {
    VkDeviceSize    BufferSize              = sizeof(uint32_t) * tSize;
    VkBuffer        StagingBuffer;
    VkDeviceMemory  StagingBufferMemory;
    void*           Data;

    createBuffer(
            BufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &StagingBuffer,
            &StagingBufferMemory
    );

    vkMapMemory(mLogicalGPU, StagingBufferMemory, 0, BufferSize, 0, &Data);
    memcpy(Data, tIndices, static_cast <size_t>(BufferSize));
    vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

    createBuffer(
            BufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &mIndexBuffer,
            &mIndexBufferMemory
    );

    copyBuffer(tTransferQueue, tTransferCommandPool, StagingBuffer, mIndexBuffer, BufferSize);

    vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
}

uint32_t dPlotMeshBase::findMemoryTypeIndex(uint32_t tAllowedTypes, VkMemoryPropertyFlags tFlags) {
    VkPhysicalDeviceMemoryProperties MemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(mGPU, &MemoryProperties);

    for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++) {
        if ((tAllowedTypes & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & tFlags) == tFlags) {
            return i;
        }
    }
}
void dPlotMeshBase::createBuffer(VkDeviceSize tBufferSize, VkBufferUsageFlags tUsageFlags, VkMemoryPropertyFlags tPropertyFlags, VkBuffer* tBuffer, VkDeviceMemory* tMemory) {
    VkBufferCreateInfo BufferCreateInfo {
            .sType      = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext          = nullptr,
            .flags          = 0,
            .size           = tBufferSize,
            .usage          = tUsageFlags,
            .sharingMode    = VK_SHARING_MODE_EXCLUSIVE
    };

    if (vkCreateBuffer(mLogicalGPU, &BufferCreateInfo, nullptr, tBuffer)) {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements MemoryRequirements;
    vkGetBufferMemoryRequirements(mLogicalGPU, *tBuffer, &MemoryRequirements);

    VkMemoryAllocateInfo MemoryAllocateInfo {
            .sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext              = nullptr,
            .allocationSize     = MemoryRequirements.size,
            .memoryTypeIndex    = findMemoryTypeIndex(MemoryRequirements.memoryTypeBits, tPropertyFlags)
    };

    if (vkAllocateMemory(mLogicalGPU, &MemoryAllocateInfo, nullptr, tMemory)) {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(mLogicalGPU, *tBuffer, *tMemory, 0);
}
void dPlotMeshBase::copyBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, VkBuffer tScrBuffer, VkBuffer tDstBuffer, VkDeviceSize tBufferSize) {
    VkCommandBuffer TransferCommandBuffer;

    VkCommandBufferAllocateInfo AllocateInfo {
            .sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext                  = nullptr,
            .commandPool            = tTransferCommandPool,
            .level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount     = 1
    };

    vkAllocateCommandBuffers(mLogicalGPU, &AllocateInfo, &TransferCommandBuffer);

    VkCommandBufferBeginInfo BeginInfo {
            .sType  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext  = nullptr,
            .flags  = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(TransferCommandBuffer, &BeginInfo);

    {
        VkBufferCopy BufferCopyRegion{
                .srcOffset  = 0,
                .dstOffset  = 0,
                .size       = tBufferSize
        };

        vkCmdCopyBuffer(TransferCommandBuffer, tScrBuffer, tDstBuffer, 1, &BufferCopyRegion);
    }

    vkEndCommandBuffer(TransferCommandBuffer);

    VkSubmitInfo SubmitInfo {
            .sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext                  = nullptr,
            .waitSemaphoreCount     = 0,
            .pWaitSemaphores        = nullptr,
            .pWaitDstStageMask      = nullptr,
            .commandBufferCount     = 1,
            .pCommandBuffers        = &TransferCommandBuffer
    };

    vkQueueSubmit(tTransferQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(tTransferQueue);

    vkFreeCommandBuffers(mLogicalGPU, tTransferCommandPool, 1, &TransferCommandBuffer);
}