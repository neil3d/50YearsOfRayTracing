# Project Setup

1. 使用 vcpkg 安装依赖项
2. 记得执行"./vcpkg integrate install"
    - 这样在 Windows + Visual Studio 就 OK 了
    - 在 MacOS + VSCode 的话，需要手动编辑 settings.json
    ```
    "cmake.configureSettings": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_TOOLCHAIN_FILE": "<path to vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake"
    }
    ```

## RTKit2: Embree & Dependencies

- For Windows 10:
  - 使用 vcpkg 安装 embree3
- For MacOS
    1. Use package installer: [embree-3.12.1.x86_64.pkg](https://github.com/embree/embree/releases/download/v3.12.1/embree-3.12.1.x86_64.pkg)
    2. Setup MacPorts: https://www.macports.org/install.php
    3. Install Intel TBB using MacPorts:
    ```
    sudo port install tbb
    ```    
