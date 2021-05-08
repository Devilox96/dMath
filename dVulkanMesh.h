//
// Created by devilox on 11/20/20.
//
//-----------------------------//
#ifndef DPLOT_DVULKANMESH_H
#define DPLOT_DVULKANMESH_H
//-----------------------------//
#include <vector>
#include <cstring>
//-----------------------------//
#include <vulkan/vulkan.h>
//-----------------------------//
class dVulkanMesh {
public:
    struct Vertex {
        float Positions[3];
        float Colors[3];
    };

    //----------//

    dVulkanMesh(VkPhysicalDevice tGPU,                  VkDevice tLogicalGPU,
                VkQueue tTransferQueue,                 VkCommandPool tTransferCommandPool,
                const std::vector <Vertex>& tVertices,  const std::vector <uint32_t>& tIndices);

    //----------//

    size_t getVertexCount();
    VkBuffer getVertexBuffer();

    size_t getIndexCount();
    VkBuffer getIndexBuffer();

    void destroyBuffers();
private:
    size_t              mVertexCount;
    VkBuffer            mVertexBuffer           = VK_NULL_HANDLE;
    VkDeviceMemory      mVertexBufferMemory     = VK_NULL_HANDLE;

    size_t              mIndexCount;
    VkBuffer            mIndexBuffer            = VK_NULL_HANDLE;
    VkDeviceMemory      mIndexBufferMemory      = VK_NULL_HANDLE;

    VkPhysicalDevice    mGPU;
    VkDevice            mLogicalGPU;

    //----------//

    void createVertexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const std::vector <Vertex>& tVertices);
    void createIndexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const std::vector <uint32_t>& tIndices);

    uint32_t findMemoryTypeIndex(uint32_t tAllowedTypes, VkMemoryPropertyFlags tFlags);
    void createBuffer(VkDeviceSize tBufferSize, VkBufferUsageFlags tUsageFlags, VkMemoryPropertyFlags tPropertyFlags, VkBuffer* tBuffer, VkDeviceMemory* tMemory);
    void copyBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, VkBuffer tScrBuffer, VkBuffer tDstBuffer, VkDeviceSize tBufferSize);
};
//-----------------------------//
#endif
