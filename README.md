![Banner](docs/sources/banner.png)

## Overview
The **Mighty Rendering Guide** is a comprehensive collection of samples and demos focused on custom rendering techniques in Unreal Engine 5. This repository serves as a practical learning resource for developers looking to master advanced rendering concepts in UE5.
Maintained by **[Something Mighty](https://www.somethingmighty.com/)**, this guide addresses the general lack of accessible, consolidated rendering resources by providing working examples that can be tested in a proper context, rather than viewing isolated code snippets.

## Features
- **Practical Examples**: Multiple sample implementations of custom rendering techniques
- **Complete Projects**: Ready-to-run UE5 projects demonstrating each concept
- **UE5.4+ Compatible**: Designed for the latest Unreal Engine versions

## Getting Started
1. Clone this repository
2. Open the file with Unreal Engine 5.4 `MightyRenderingGuide.uproject`
3. Explore the sample scenes in the Content folder
4. Review the source code in the Source directory to understand the implementation details

## Project Structure
The repository is organized into multiple examples showcasing different rendering techniques:
- **MightyRenderingGuide**: Core project module
- **MightyRenderingGuideGfx**: Custom graphics implementations
  - **DynamicMesh**: Custom geometry generated on the CPU with custom vertex data and a custom vertex shader
  - **PostProcess**: Custom post-process contour effect injected into the main rendering pipeline
  - **RenderingPass**: Custom multi-pass rendering pipeline using Timothy Lotte's CRT shader

## Community Contributions
This is a community-driven project! We welcome contributions in the following ways:
- **Open Issues**: Found a better way to implement a technique? Open an issue to discuss it!
- **Suggest Samples**: Have ideas for new rendering examples that would benefit the community? Let us know!
- **Submit PRs**: Feel free to add your own examples or improve existing ones

Our goal is to create a comprehensive, collaborative resource for UE5 rendering techniques that evolves with community knowledge and the engine itself.

## Project License
This project's source code is available under  the [MIT license](https://github.com/Something-Mighty/UE5-custom-rendering-guide/blob/main/LICENSE). You are free to use, modify, and distribute the code according to the terms specified in the license.

The Mighty Mascots assets (including but not limited to 3D models, textures, and animations) included in this repository are the intellectual property of IEC Collective UG and are provided under a limited license:

- You may use these assets for learning and testing within the Mighty Rendering Guide
- You may not use, reproduce, or distribute these assets in any other projects or contexts
- Commercial use is strictly prohibited
- Modification of these assets is not permitted
