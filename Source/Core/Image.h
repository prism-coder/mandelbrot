#pragma once

#include "Core/Core.h"

#include <filesystem>
#include <vector>

/**
 * Class representing an image loaded from a file.
 * 
 * This class is responsible for loading image data from a specified file path and providing access to the image's width, height, and pixel data.
 * The image data is stored as a vector of unsigned char, which typically represents RGBA pixel data.
 * The class also provides methods to check if the image was successfully loaded.
 */
class Image {
public:
    /**
     * Constructor that loads an image from the specified file path.
     * 
     * @param filepath The path to the image file to be loaded. The file format should be supported by the underlying image loading library (e.g., PNG, JPEG).
     */
    Image(const std::filesystem::path& filepath);

    /**
     * Destructor for the `Image` class.
     * 
     * This will automatically clean up any resources used by the image, such as the pixel data vector.
     */
    ~Image() = default;

    /**
     * Returns the width of the image in pixels.
     * 
     * @return The width of the image.
     */
    uint32_t GetWidth() const { return m_Width; }

    /**
     * Returns the height of the image in pixels.
     * 
     * @return The height of the image.
     */
    uint32_t GetHeight() const { return m_Height; }

    /**
     * Returns a pointer to the image's pixel data.
     * 
     * @return A pointer to the image's pixel data.
     */
    const unsigned char* GetPixels() const { return m_Pixels.data(); }

    /**
     * Checks if the image was successfully loaded by verifying that the pixel data vector is not empty.
     * 
     * @return `true` if the image was loaded successfully, `false` otherwise.
     */
    bool IsLoaded() const { return !m_Pixels.empty(); }
private:
    /// @brief The width of the image in pixels.
    uint32_t m_Width = 0;

    /// @brief The height of the image in pixels.
    uint32_t m_Height = 0;

    /// @brief The pixel data of the image, stored as a vector of unsigned char. Each pixel is typically represented as RGBA (4 bytes per pixel).
    std::vector<unsigned char> m_Pixels;
};