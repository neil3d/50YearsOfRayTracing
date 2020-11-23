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