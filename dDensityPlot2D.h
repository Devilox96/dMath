//
// Created by devilox on 5/1/21.
//
//-----------------------------//
#include <array>
#include <iostream>
#include <random>
//-----------------------------//
#include "dPlotMeshBase.h"
//-----------------------------//
#ifndef DPLOT_DDENSITYPLOT2D_H
#define DPLOT_DDENSITYPLOT2D_H
//-----------------------------//
template <size_t Horiz, size_t Vert>
class dDensityPlot2D : public dPlotMeshBase {
public:
    dDensityPlot2D(float tPosX,                             float tPosY,                            float tPosZ,
                   float tWidth,                            float tHeight,
                   VkPhysicalDevice tGPU,                   VkDevice tLogicalGPU,
                   VkQueue tTransferQueue,                  VkCommandPool tTransferCommandPool) {
        mGPU            = tGPU;
        mLogicalGPU     = tLogicalGPU;
        mTransferQueue  = tTransferQueue;
        mCommandPool    = tTransferCommandPool;

        //----------//

        std::uniform_real_distribution<float> distribution(0.0, 1.0);


        for (size_t iRow = 0; iRow < Vert; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                mVertices[iRow * Horiz + iColumn].mPos[0]       = tWidth / float(Horiz - 1) * float(iColumn) + tPosX;   //---x---//
                mVertices[iRow * Horiz + iColumn].mPos[1]       = tHeight / float(Vert - 1) * float(iRow) + tPosY;      //---y---//
                mVertices[iRow * Horiz + iColumn].mPos[2]       = tPosZ;                                                //---z---//

                mVertices[iRow * Horiz + iColumn].mColor[0]     = 1.0f;                                                 //---red---//
                mVertices[iRow * Horiz + iColumn].mColor[1]     = 1.0f;                                                 //---green---//
                mVertices[iRow * Horiz + iColumn].mColor[2]     = 1.0f;                                                 //---blue---//

                mMesh[iRow][iColumn] = distribution(generator);
            }
        }

        for (size_t iRow = 0; iRow < Vert - 1; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz - 1; iColumn++) {
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6] = iRow * Horiz + iColumn;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 1] = iRow * Horiz + iColumn + 1;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 2] = (iRow + 1) * Horiz + iColumn;

                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 3] = (iRow + 1) * Horiz + iColumn;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 4] = iRow * Horiz + iColumn + 1;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 5] = (iRow + 1) * Horiz + iColumn + 1;
            }
        }
    }

    //----------//

    void addColor(float tVal, float tRed, float tGreen, float tBlue) {
        if (mGradient.empty()) {
            mGradient.emplace_back(ColorPoint{tVal, tRed, tGreen, tBlue});
        } else {
            if (tVal > mGradient[mGradient.size() - 1].mVal) {
                mGradient.emplace_back(ColorPoint{tVal, tRed, tGreen, tBlue});
            } else if (tVal < mGradient[0].mVal) {
                mGradient.emplace(mGradient.begin(), ColorPoint{tVal, tRed, tGreen, tBlue});
            } else {
                auto Iter = mGradient.begin();

                for (size_t i = 0; i < mGradient.size() - 1; i++, Iter++) {
                    if (tVal > mGradient[i].mVal && tVal < mGradient[i + 1].mVal) {
                        mGradient.emplace(Iter + 1, ColorPoint{tVal, tRed, tGreen, tBlue});
                        break;
                    }
                }
            }
        }
    }

    void generateBuffers() {
        if (!mGradient.empty()) {
            for (size_t iRow = 0; iRow < Vert; iRow++) {
                for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                    if (mMesh[iRow][iColumn] >= mGradient.back().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.back().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.back().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.back().mColor[2];

                        continue;
                    }

                    if (mMesh[iRow][iColumn] <= mGradient.front().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.front().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.front().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.front().mColor[2];

                        continue;
                    }

                    for (size_t i = 0; i < mGradient.size() - 1; i++) {
                        if (mMesh[iRow][iColumn] > mGradient[i].mVal && mMesh[iRow][iColumn] < mGradient[i + 1].mVal) {
                            float Multiplier = (mMesh[iRow][iColumn] - mGradient[i].mVal) / (mGradient[i + 1].mVal - mGradient[i].mVal);

                            mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient[i].mColor[0] + (mGradient[i + 1].mColor[0] - mGradient[i].mColor[0]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient[i].mColor[1] + (mGradient[i + 1].mColor[1] - mGradient[i].mColor[1]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient[i].mColor[2] + (mGradient[i + 1].mColor[2] - mGradient[i].mColor[2]) * Multiplier;

                            break;
                        }
                    }
                }
            }
        }

        createVertexBuffer(mTransferQueue, mCommandPool, mVertices.data(), mVertices.size());
        createIndexBuffer(mTransferQueue, mCommandPool, mIndices.data(), mIndices.size());
    }

    //----------//

    size_t getVertexCount() {
        return mVertices.size();
    }
    size_t getIndexCount() {
        return mIndices.size();
    }

    //----------//

    void randomize(size_t iNum) {
        static bool Test = false;

        std::normal_distribution<float> distribution(1.0, 1.0);

        for (size_t iRow = 0; iRow < Vert; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                mMesh[iRow][iColumn] = distribution(generator);
            }
        }

        if (!mGradient.empty()) {
            for (size_t iRow = 0; iRow < Vert; iRow++) {
                for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                    if (mMesh[iRow][iColumn] >= mGradient.back().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.back().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.back().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.back().mColor[2];

                        continue;
                    }

                    if (mMesh[iRow][iColumn] <= mGradient.front().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.front().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.front().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.front().mColor[2];

                        continue;
                    }

                    for (size_t i = 0; i < mGradient.size() - 1; i++) {
                        if (mMesh[iRow][iColumn] > mGradient[i].mVal && mMesh[iRow][iColumn] < mGradient[i + 1].mVal) {
                            float Multiplier = (mMesh[iRow][iColumn] - mGradient[i].mVal) / (mGradient[i + 1].mVal - mGradient[i].mVal);

                            mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient[i].mColor[0] + (mGradient[i + 1].mColor[0] - mGradient[i].mColor[0]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient[i].mColor[1] + (mGradient[i + 1].mColor[1] - mGradient[i].mColor[1]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient[i].mColor[2] + (mGradient[i + 1].mColor[2] - mGradient[i].mColor[2]) * Multiplier;

                            break;
                        }
                    }
                }
            }
        }

        //----------//

        VkDeviceSize    BufferSize              = sizeof(Vertex) * mVertices.size();
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
        memcpy(Data, mVertices.data(), static_cast <size_t>(BufferSize));
        vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

        copyBuffer(mTransferQueue, mCommandPool, StagingBuffer, mVertexBuffers[iNum], BufferSize);

        vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
        vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
    }

    //----------//

    ///---TODO: add size check---///
    void updateValues(const std::vector <std::vector <float>>& tNewData, size_t iNum) {
        for (size_t iRow = 0; iRow < Vert; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                mMesh[iRow][iColumn] = tNewData[iColumn][iRow];
            }
        }

        if (!mGradient.empty()) {
            for (size_t iRow = 0; iRow < Vert; iRow++) {
                for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                    if (mMesh[iRow][iColumn] >= mGradient.back().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.back().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.back().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.back().mColor[2];

                        continue;
                    }

                    if (mMesh[iRow][iColumn] <= mGradient.front().mVal) {
                        mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient.front().mColor[0];
                        mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient.front().mColor[1];
                        mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient.front().mColor[2];

                        continue;
                    }

                    for (size_t i = 0; i < mGradient.size() - 1; i++) {
                        if (mMesh[iRow][iColumn] > mGradient[i].mVal && mMesh[iRow][iColumn] < mGradient[i + 1].mVal) {
                            float Multiplier = (mMesh[iRow][iColumn] - mGradient[i].mVal) / (mGradient[i + 1].mVal - mGradient[i].mVal);

                            mVertices[iRow * Horiz + iColumn].mColor[0] = mGradient[i].mColor[0] + (mGradient[i + 1].mColor[0] - mGradient[i].mColor[0]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[1] = mGradient[i].mColor[1] + (mGradient[i + 1].mColor[1] - mGradient[i].mColor[1]) * Multiplier;
                            mVertices[iRow * Horiz + iColumn].mColor[2] = mGradient[i].mColor[2] + (mGradient[i + 1].mColor[2] - mGradient[i].mColor[2]) * Multiplier;

                            break;
                        }
                    }
                }
            }
        }

        //----------//

        VkDeviceSize    BufferSize              = sizeof(Vertex) * mVertices.size();
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
        memcpy(Data, mVertices.data(), static_cast <size_t>(BufferSize));
        vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

        copyBuffer(mTransferQueue, mCommandPool, StagingBuffer, mVertexBuffers[iNum], BufferSize);

        vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
        vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
    }
private:
    struct ColorPoint {
        float mVal;
        float mColor[3];
    };

    //----------//

    std::array <std::array <float, Horiz>, Vert>                mMesh;

    std::array <Vertex, Horiz * Vert>                           mVertices;
    std::array <uint32_t, (Horiz - 1) * (Vert - 1) * 3 * 2>     mIndices;                                               //---(n-1 * (n-1) quads that consist of 2 triangles drawn with 6 indices---//

    std::vector <ColorPoint>                                    mGradient;


    std::default_random_engine generator;
};
//-----------------------------//
#endif
