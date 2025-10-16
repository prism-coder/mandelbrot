#include "Image.h"

#include "Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

Image::Image(const std::filesystem::path& filepath) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 4);
    stbi_set_flip_vertically_on_load(1);

    if (data) {
        m_Width = width;
        m_Height = height;
        size_t size = m_Width * m_Height * 4;
        m_Pixels.assign(data, data + size);
        stbi_image_free(data);
    } else {
        Log::Error("Image::Image - Failed to load image: " + filepath.string());
    }
}
