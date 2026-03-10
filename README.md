<div align="center">
  <img src="Internal/Icons/AppIcon/appicon.png" alt="Mandelbrot++ Logo" width="140"/>

  # Mandelbrot++

  *A GPU-accelerated fractal explorer built on a custom C++20 rendering engine.*

  <br/>

  ![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=flat&logo=cplusplus&logoColor=white)
  ![OpenGL](https://img.shields.io/badge/OpenGL-4.6-5586A4?style=flat&logo=opengl&logoColor=white)
  ![Platform](https://img.shields.io/badge/Windows-x64-0078D4?style=flat&logo=windows&logoColor=white)
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat)](LICENSE)

  ![Premake](https://img.shields.io/badge/Build-Premake5-E8572A?style=flat&logo=cmake&logoColor=white)
  ![Visual Studio 2022](https://img.shields.io/badge/Visual%20Studio-2022-5C2D91?style=flat&logo=visualtudio&logoColor=white)
  ![Visual Studio 2026](https://img.shields.io/badge/Visual%20Studio-2026-5C2D91?style=flat&logo=visualstudio&logoColor=white)
  ![ImGui](https://img.shields.io/badge/GUI-ImGui-9B59B6?style=flat)
  ![GPU](https://img.shields.io/badge/Renderer-GPU%20Shaders-76B900?style=flat&logo=nvidia&logoColor=white)

</div>

## Screenshots

<div align="center">

| Default                                       | Elephant Valley                                       |
|:---------------------------------------------:|:-----------------------------------------------------:|
| ![Default Configuration](./Screenshots/0.png) | ![Elephant Valley Configuration](./Screenshots/1.png) |

| Serpent's Eye Julia                                       | Electric Seahorse                                       |
|:---------------------------------------------------------:|:-------------------------------------------------------:|
| ![Serpent's Eye Julia Configuration](./Screenshots/2.png) | ![Electric Seahorse Configuration](./Screenshots/3.png) |

| Project & Settings                                   |
|:----------------------------------------------------:|
| ![Project and Settings Windows](./Screenshots/4.png) |

</div>

## What is this?

**Mandelbrot++** is an interactive explorer for the **Mandelbrot set** and related fractals, rendered entirely on the **GPU** via `GLSL` *shaders*. It started as an exercise in graphics programming and grew into a showcase of how complex mathematical domains (*fractal algorithms*, *coloring theory*, *coordinate transformations*) can be abstracted into clean, decoupled systems without sacrificing performance.

The result is an application that runs at **interactive frame rates** even at high iteration counts, while remaining straightforward to extend with new algorithms, coloring modes, or export formats.

## Architecture

The engine is organized into independent, single-responsibility layers. Each concern (*rendering*, *serialization*, *UI*, *input*) lives in its own module with a clearly defined interface, making the codebase easy to navigate and extend:

```
Source/
  Core/        - Application lifecycle, window management, layer stack
  Renderer/    - OpenGL abstraction (shaders, framebuffers, draw calls)
  Layers/      - Fractal logic layer, UI layer; composed at runtime
  Editor/      - ImGui panels (Inspector, Viewport, Project, Settings...)
  Utilities/   - Serialization, file I/O, image export
  Platform/    - Platform-specific entry points
```

Key design decisions:
- **GPU-first computation**: all fractal math lives in `GLSL` *fragment shaders*. The CPU only manages state and uploads uniforms, keeping the main thread free.
- **Layer stack**: rendering and UI are separate layers composed by the application, following a pattern similar to game engine architecture.
- **Data-driven configuration**: fractal parameters, view state, coloring, and orbit traps are fully described in `YAML` and round-trip cleanly through a typed serialization layer.
- **Shader hot-path isolation**: coloring algorithms (*Step*, *Smooth*, *Distance Estimation*) and orbit trap types are self-contained shader modules, making it straightforward to add new ones without touching unrelated code.

## Features

### Fractal Algorithms
- `Mandelbrot`, `Burning Ship`, and `Tricorn`
- Arbitrary power exponents (Multibrot)
- Julia set mode with live parameter tuning

### Coloring System
- **Step**: classic banded appearance
- **Smooth**: continuous gradient via normalized iteration count
- **Distance Estimation**: analytically-derived boundary lighting
- Orbit coloring (*angle-based*) and orbit traps (`Point`, `Circle`, `Line`, `Box`, `Cross`)
- Fully customizable color palettes with frequency and phase offset

### User Interface
- Dockable ImGui layout: `Inspector`, `Viewport`, `Project`, `Statistics`, `Settings`, `About`
- 40+ built-in themes
- Configurable font size and UI scale

### Configuration & Export
- Save and load configurations as `YAML` files
- Hundreds of curated presets organized by category
- High-resolution image export
- Recent files list

## Building and Running

### Prerequisites
- Windows 10/11, Visual Studio 2022 or 2026, Git

### Setup

1. Clone the repository recursively to retrieve libraries

   ```bash
   git clone --recursive https://github.com/prism-coder/mandelbrot
   ```

2. Navigate to the newly created repository folder

   ```bash
   cd mandelbrot
   ```

3. Run the setup script for your Visual Studio version:

   ```bash
   Scripts/Setup/Windows-vs2022.bat   # VS 2022
   Scripts/Setup/Windows-vs2026.bat   # VS 2026
   ```

4. Open `Mandelbrot.sln` (or `Mandelbrot.slnx` depending on the version you used) in Visual Studio, select a configuration, and build.

### Build Configurations

| Configuration   | Description                               |
|-----------------|-------------------------------------------|
| `Debug`         | Full debug symbols, no optimization       |
| `Release`       | Optimized with symbols                    |
| `Dist`          | Fully optimized, windowed app, no console |

## Configuration File Format

Configurations are plain `YAML` and can be shared, version-controlled, or hand-edited:

```yaml
Mandelbrot:
  FractalParameters:
    Algorithm: Mandelbrot   # Mandelbrot | BurningShip | Tricorn
    Power: 2.0
    Bailout: 16.0
    MaxIterations: 256
  ViewParameters:
    Zoom: 1.0
    Position: [-0.5, 0.0]
    Rotation: 0.0
  JuliaParameters:
    JuliaMode: false
    JuliaC: [-0.8, 0.156]
  ColoringParameters:
    ExteriorColoring: Smooth  # Step | Smooth | DistanceEstimation
    InteriorColoring: Black   # Black | White | CustomColor
    InteriorColor: [0.0, 0.0, 0.0]
    ColorFrequency: 1.0
    ColorOffset: 0.0
    OrbitColoring: false
    DistanceScale: 50.0
    ColorPalette:
      Colors:
        - [0.0, 0.0, 0.0]
        - [1.0, 0.0, 0.0]
        - [1.0, 1.0, 0.0]
        - [0.0, 1.0, 0.0]
  OrbitTrap:
    Type: None   # None | Point | Circle | Line | Box | Cross
    P1: [0.0, 0.0]
    P2: [0.0, 0.0]
    Color: [1.0, 1.0, 1.0]
    Blend: 0.5
```
## Contributing

Pull requests, bug reports, and feature suggestions are welcome.

- Use the `Debug` configuration for development
- Follow the existing module boundaries when adding features
- Document non-obvious shader math inline

## License

MIT - see [LICENSE](LICENSE) for details.

## Acknowledgments

- **Fractal mathematics:** Benoît Mandelbrot and the broader research community
- **Libraries:** `GLFW`, `ImGui`, `GLM`, `yaml-cpp`, `stb`, `GLAD`.

> **Note**: *Requires a GPU with OpenGL 4.6 support. Integrated graphics may see reduced frame rates at high iteration counts.*
