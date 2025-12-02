
<div align="center">

<img src="https://your-cdn.com/ignition-link-logo.svg" alt="Ignition Link Logo" width="150"/>

# Ignition Link

### The Real-Time C++ Communication & Control Fabric for Intelligent Edge Devices

**`Ignition Link`** is a lightweight, high-performance C++ library that provides the core "nervous system" for modern robotics and IoT applications. It's a production-grade toolkit for building complex, multi-process systems that are reliable, scalable, and blazingly fast.

---

[![Build Status](https://img.shields.io/github/actions/workflow/status/your-org/ignition-link/build.yml?branch=main&style=for-the-badge)](https://github.com/your-org/ignition-link/actions)
[![License](https://img.shields.io/github/license/your-org/ignition-link?style=for-the-badge)](./LICENSE)
[![Discord](https://img.shields.io/discord/81384788765712384.svg?style=for-the-badge&logo=discord&color=7289DA&label=Join%20Chat)](https://discord.gg/your-invite-link)
[![Documentation](https://img.shields.io/badge/Documentation-View_Docs-blue?style=for-the-badge)](https://docs.ignition-hub.ai/link)

</div>

## The Problem: Moving Beyond Prototypes

The world of robotics and IoT is filled with powerful sensors and intelligent AI models, but the software connecting them is often a bottleneck. Prototyping tools like ROS are fantastic for research, but they lack the hard real-time guarantees, security, and lightweight footprint required for commercial, production-grade systems.

Every serious robotics company is forced to spend years and millions of dollars "re-inventing the wheel" by building their own custom messaging, data logging, and fleet management infrastructure from scratch.

## The Solution: A Production-Ready Foundation

`Ignition Link` solves this problem. It provides the essential, non-differentiating infrastructure that all intelligent edge devices need, allowing you to focus on your unique application logic. It is the spiritual successor to ROS, redesigned from the ground up for performance, reliability, and the demands of the modern AI-powered edge.

### Core Features

*   🧠 **High-Performance Messaging:** A real-time, publish-subscribe message bus that uses shared memory for near-zero-latency communication between processes on a single device.
*   ☁️ **Secure Fleet Connectivity:** A robust, resilient client for securely connecting your device to a cloud backend (like our **[Ignition Fleet](https://ignition-hub.ai/fleet)**) for OTA updates, telemetry, and remote commands.
*   💾 **Intelligent Data Logistics:** A powerful "flight recorder" system to intelligently capture and upload high-value data from the field, fueling your AI development flywheel.
*   🔌 **Hardware Abstraction Layer (HAL):** A clean, standardized set of C++ interfaces for common hardware (cameras, IMUs, CAN Bus), making your application code portable and easy to test.

## Architectural Overview

`Ignition Link` enables a clean, decoupled architecture where independent nodes (processes) communicate through a central, high-performance message bus.

```
      +-------------------------------------------------+
      |        Your Robotics Application (Nodes)        |
      | +------------+  +------------+  +-------------+ |
      | | Perception |  | Navigation |  | Motor Control | |
      | +------------+  +------------+  +-------------+ |
      +-------▲--------------▲---------------▲---------+
              |              |               |
      +-------▼--------------▼---------------▼---------+
      |        ignlink::msg (The Message Bus)         |
      +-------▲--------------▲---------------▲---------+
              |              |               |
      +-------▼--------------▼---------------▼---------+
      |       ignlink::hal   | ignlink::data | ignlink::fleet |
      | (Hardware Interface) | (Data Logger) | (Cloud Client) |
      +----------------------+---------------+----------------+
              |                      |               |
              ▼                      ▼               ▼
      +----------------+     +---------------+  +------------+
      | Physical Sensors |     | Local Storage |  | The Cloud  |
      +----------------+     +---------------+  +------------+
```

## Getting Started

`Ignition Link` is a C++17 library built with CMake.

### Prerequisites

*   A C++17 compliant compiler (GCC 9+, Clang 10+)
*   CMake (3.16+)

### Quick Start: Building the Examples

1.  **Clone the repository:**
    ```bash
    git clone --recurse-submodules https://github.com/your-org/ignition-link.git
    cd ignition-link
    ```
    *(The `--recurse-submodules` flag is important to pull in third-party dependencies.)*

2.  **Configure and Build:**
    ```bash
    mkdir build && cd build
    cmake ..
    make -j$(nproc)
    ```

3.  **Run an Example:**
    The compiled example binaries will be in the `build/examples/` directory.
    ```bash
    ./examples/01_hello_world/hello_world_app
    ```

### Using `Ignition Link` in Your Own Project

The easiest way to use `Ignition Link` is with CMake's `FetchContent` module.

```cmake
# In your project's CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  ignition_link
  GIT_REPOSITORY https://github.com/your-org/ignition-link.git
  GIT_TAG        main # Or a specific release tag
)

FetchContent_MakeAvailable(ignition_link)

# ... then link it to your target
target_link_libraries(my_awesome_robot PRIVATE ignition-link::ignlink)
```

## The Ignition AI Ecosystem

`Ignition Link` is a core component of the broader Ignition AI ecosystem for building and deploying high-performance AI.

*   **[Ignition Hub](https://ignition-hub.ai):** The cloud platform for compiling your AI models into hyper-performant TensorRT engines.
*   **xInfer Runtime:** The on-device library for running your optimized AI models with maximum speed.
*   **xTorch:** A C++ training library for a complete, end-to-end high-performance workflow.

`Ignition Link` is the nervous system that connects the `xInfer` "brain" to the rest of your device and the cloud.

## Contributing

We are building `Ignition Link` in the open and welcome contributions from the community! Whether it's a bug report, a new feature, or a new hardware implementation for the HAL, we'd love your help.

Please read our **[CONTRIBUTING.md](./CONTRIBUTING.md)** guide to get started.

## License

`Ignition Link` is licensed under the **[Apache 2.0 License](./LICENSE)**.