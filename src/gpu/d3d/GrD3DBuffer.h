/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrD3DBuffer_DEFINED

#define GrD3DBuffer_DEFINED

#include "include/gpu/d3d/GrD3DTypes.h"
#include "src/gpu/GrGpuBuffer.h"
#include "src/gpu/GrManagedResource.h"

class GrD3DGpu;

class GrD3DBuffer : public GrGpuBuffer {
public:
    static sk_sp<GrD3DBuffer> Make(GrD3DGpu*, size_t size, GrGpuBufferType, GrAccessPattern);

    ~GrD3DBuffer() override {}

    ID3D12Resource* d3dResource() const {
        SkASSERT(fD3DResource);
        return fD3DResource.Get();
    }

    void setResourceState(const GrD3DGpu* gpu, D3D12_RESOURCE_STATES newResourceState);

protected:
    GrD3DBuffer(GrD3DGpu*, size_t size, GrGpuBufferType, GrAccessPattern, ComPtr<ID3D12Resource>,
                D3D12_RESOURCE_STATES);

    void onAbandon() override;
    void onRelease() override;

    D3D12_RESOURCE_STATES fResourceState;

private:
    void onMap() override;
    void onUnmap() override;
    bool onUpdateData(const void* src, size_t srcSizeInBytes) override;

    void internalMap(size_t size);
    void internalUnmap(size_t size);

    void validate() const;

    GrD3DGpu* getD3DGpu() const {
        SkASSERT(!this->wasDestroyed());
        return reinterpret_cast<GrD3DGpu*>(this->getGpu());
    }

    ComPtr<ID3D12Resource> fD3DResource;
    ID3D12Resource* fStagingBuffer = nullptr;
    size_t fStagingOffset = 0;

    using INHERITED = GrGpuBuffer;
};

#endif
