//
// Created by devilox on 11/20/20.
//
//-----------------------------//
#include <stdexcept>
#include "dVulkanMesh.h"
//-----------------------------//
dVulkanMesh::dVulkanMesh(   VkPhysicalDevice tGPU,                  VkDevice tLogicalGPU,
                            VkQueue tTransferQueue,                 VkCommandPool tTransferCommandPool,
                            const std::vector <Vertex>& tVertices,  const std::vector <uint32_t>& tIndices) {
    mGPU = tGPU;
    mLogicalGPU = tLogicalGPU;

    mVertexCount = tVertices.size();
    mIndexCount = tIndices.size();

    createVertexBuffer(tTransferQueue, tTransferCommandPool, tVertices);
    createIndexBuffer(tTransferQueue, tTransferCommandPool, tIndices);
}
//-----------------------------//
size_t dVulkanMesh::getVertexCount() {
    return mVertexCount;
}
VkBuffer dVulkanMesh::getVertexBuffer() {
    return mVertexBuffer;
}

size_t dVulkanMesh::getIndexCount() {
    return mIndexCount;
}
VkBuffer dVulkanMesh::getIndexBuffer() {
    return mIndexBuffer;
}

void dVulkanMesh::destroyBuffers() {
    vkDestroyBuffer(mLogicalGPU, mVertexBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, mVertexBufferMemory, nullptr);
    vkDestroyBuffer(mLogicalGPU, mIndexBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, mIndexBufferMemory, nullptr);
}
//-----------------------------//
void dVulkanMesh::createVertexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const std::vector <Vertex>& tVertices) {
    VkDeviceSize    BufferSize              = sizeof(Vertex) * mVertexCount;
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
    memcpy(Data, tVertices.data(), static_cast <size_t>(BufferSize));
    vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

    createBuffer(
            BufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            &mVertexBuffer,
            &mVertexBufferMemory
            );

    copyBuffer(tTransferQueue, tTransferCommandPool, StagingBuffer, mVertexBuffer, BufferSize);

    vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
    vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
}
void dVulkanMesh::createIndexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const std::vector <uint32_t>& tIndices) {
    VkDeviceSize    BufferSize              = sizeof(uint32_t) * mIndexCount;
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
    memcpy(Data, tIndices.data(), static_cast <size_t>(BufferSize));
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

uint32_t dVulkanMesh::findMemoryTypeIndex(uint32_t tAllowedTypes, VkMemoryPropertyFlags tFlags) {
    VkPhysicalDeviceMemoryProperties MemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(mGPU, &MemoryProperties);

    for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++) {
        if ((tAllowedTypes & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & tFlags) == tFlags) {
            return i;
        }
    }
}
void dVulkanMesh::createBuffer(VkDeviceSize tBufferSize, VkBufferUsageFlags tUsageFlags, VkMemoryPropertyFlags tPropertyFlags, VkBuffer* tBuffer, VkDeviceMemory* tMemory) {
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
void dVulkanMesh::copyBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, VkBuffer tScrBuffer, VkBuffer tDstBuffer, VkDeviceSize tBufferSize) {
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