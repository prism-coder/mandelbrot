#pragma once

#include <glm/glm.hpp>
#include <vector>

// The maximum number of colors that our Shader will support
constexpr int MAX_PALETTE_COLORS = 16;

struct Palette {
    // The 'Colors' vector can be any size on the CPU for UI flexibility.
    std::vector<glm::vec3> Colors;

    // These arrays are the ones we will actually send to the shader.
    // They have a fixed size and we prepare them before rendering.
    glm::vec3 ColorData[MAX_PALETTE_COLORS] = {};
    float ColorPositions[MAX_PALETTE_COLORS] = {};
    int ColorCount = 0;

    Palette() {
        // We initialize with a default palette (blue and yellow)
        Colors.push_back({ 0.0f, 0.0f, 0.5f }); // Dark blue
        Colors.push_back({ 0.0f, 0.2f, 1.0f }); // Bright blue
        Colors.push_back({ 1.0f, 1.0f, 0.0f }); // Yellow
        Colors.push_back({ 0.0f, 0.0f, 0.0f }); // Black (for the interior)
    }

    // This function prepares the data to be sent to the GPU.
    void PrepareForShader() {
        ColorCount = static_cast<int>(Colors.size());

        if (ColorCount > MAX_PALETTE_COLORS) {
            ColorCount = MAX_PALETTE_COLORS; // Truncate if there are too many colors
        }

        for (int i = 0; i < ColorCount; ++i) {
            ColorData[i] = Colors[i];
            // Distribute the color positions evenly between 0.0 and 1.0
            ColorPositions[i] = static_cast<float>(i) / static_cast<float>(ColorCount - 1);
        }
    }
};