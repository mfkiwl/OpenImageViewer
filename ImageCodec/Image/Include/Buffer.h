#pragma once
#include <cmath>
#include <cstdint>
#include <memory>

class Buffer
{
public:
    static constexpr int Alignment = 16;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer() = default;


    void operator=(Buffer&& rhs)
    {
        Swap(std::move(rhs));
    }

    Buffer(Buffer&& rhs)
    {
        Swap(std::move(rhs));
    }


    const std::byte* GetBuffer() const
    {
        return fData;
    }

    std::byte* GetBuffer()
    {
        return fData;
    }

    void Free()
    {
        FreeImpl();
    }

    void Allocate(size_t size)
    {
        AllocateImp(size);
    }

    void Read(std::byte* dest, size_t offset, size_t size) const
    {
        if (offset + size <= fSize)
            memcpy(dest ,fData + offset, size);
        else
            throw std::runtime_error("Memory read overflow");
    }

    void Write(const std::byte* buffer, size_t offset, size_t size)
    {
        if (offset + size <= fSize)
            memcpy(fData + offset, buffer, size);
        else
            throw std::runtime_error("Memory write overflow");
    }

    ~Buffer()
    {
        Free();
    }
    
    size_t Size() const
    {
        return fSize;
    }
private:
    // private methods
    void Swap(Buffer&& rhs)
    {
        std::swap(fSize, rhs.fSize);
        rhs.fSize = 0;
        std::swap(fData, rhs.fData);
        _aligned_free(rhs.fData);
        rhs.fData = nullptr;
    }

    void AllocateImp(size_t size)
    {
        Free();
        fData = reinterpret_cast<std::byte*>(_aligned_malloc(size, Alignment));
        fSize = size;
    }

    void FreeImpl()
    {
        if (fData != nullptr)
        {
            _aligned_free(fData);
            fData = nullptr;
            fSize = 0;
        }
    }

    // private member fields
    std::byte* fData = nullptr;
    size_t fSize = 0;
};
