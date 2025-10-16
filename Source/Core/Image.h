#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <vector>

class Image {
public:
    Image(const std::filesystem::path& filepath);
    ~Image() = default;

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    const unsigned char* GetPixels() const { return m_Pixels.data(); }
    bool IsLoaded() const { return !m_Pixels.empty(); }

private:
    uint32_t m_Width = 0, m_Height = 0;
    std::vector<unsigned char> m_Pixels;
};