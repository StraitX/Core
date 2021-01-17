cmake_minimum_required(VERSION 3.10)


set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)
set(VERSION_PATCH 20)

project(
    StraitXEngine 
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} 
    DESCRIPTION "StraitXEngine project"
    LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

include(Config)

set(SOURCES_PLATFORM_I
    ${SOURCES_DIR}/platform/io.cpp
    ${SOURCES_DIR}/platform/memory.cpp
    ${SOURCES_DIR}/platform/result.cpp
    ${SOURCES_DIR}/platform/glloader.cpp
)

set(SOURCES_CORE
    ${SOURCES_DIR}/core/assert.cpp
    ${SOURCES_DIR}/core/time_quant.cpp
    ${SOURCES_DIR}/core/memory/linear_allocator.cpp
    ${SOURCES_DIR}/core/string.cpp
)

set(SOURCES_GRAPHICS_VULKAN
    ${SOURCES_DIR}/graphics/vulkan/debug.cpp
    ${SOURCES_DIR}/graphics/vulkan/physical_device.cpp
    ${SOURCES_DIR}/graphics/vulkan/renderer_api.cpp
    ${SOURCES_DIR}/graphics/vulkan/logical_device.cpp
    ${SOURCES_DIR}/graphics/vulkan/swapchain.cpp
    ${SOURCES_DIR}/graphics/vulkan/command_pool.cpp
    ${SOURCES_DIR}/graphics/vulkan/command_buffer.cpp
    ${SOURCES_DIR}/graphics/vulkan/fence.cpp
    ${SOURCES_DIR}/graphics/vulkan/render_pass.cpp
    ${SOURCES_DIR}/graphics/vulkan/framebuffer.cpp
    ${SOURCES_DIR}/graphics/vulkan/shader.cpp
    ${SOURCES_DIR}/graphics/vulkan/graphics_pipeline.cpp
    ${SOURCES_DIR}/graphics/vulkan/buffer.cpp
    ${SOURCES_DIR}/graphics/vulkan/memory.cpp

    ${SOURCES_DIR}/graphics/vulkan/graphics_api_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/logical_gpu_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/gpu_memory_allocator.cpp
    ${SOURCES_DIR}/graphics/vulkan/gpu_buffer_impl.cpp
)

set(SOURCES_GRAPHICS_OPENGL
    ${SOURCES_DIR}/graphics/opengl/graphics_api_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/logical_gpu_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/gpu_buffer_impl.cpp
)

set(SOURCES_GRAPHICS_API
    ${SOURCES_DIR}/graphics/api/graphics_api.cpp
    ${SOURCES_DIR}/graphics/api/gpu_configuration.cpp
    ${SOURCES_DIR}/graphics/api/logical_gpu.cpp
    ${SOURCES_DIR}/graphics/api/gpu_buffer.cpp
)

set(SOURCES_GRAPHICS 
    ${SOURCES_GRAPHICS_VULKAN}
    ${SOURCES_GRAPHICS_OPENGL}
    ${SOURCES_GRAPHICS_API}
)

set(SOURCES_SERVERS
    ${SOURCES_DIR}/servers/display_server.cpp
)

set(SOURCES_MAIN
    ${SOURCES_DIR}/main/engine.cpp
    ${SOURCES_DIR}/main/application.cpp
    ${SOURCES_DIR}/main/main.cpp
)

set(LIBS_ENGINE
    glad
)

set(INCLUDES
    ${SOURCES_DIR}
    ${THIRDPARTY_DIR}/glad/include
    ${THIRDPARTY_DIR}/vulkan-sdk/include
)

if(STRAITX_PLATFORM_LINUX)

    set(SOURCES_PLATFORM_D
        ${SOURCES_DIR}/platform/linux/window_system_impl.cpp
        ${SOURCES_DIR}/platform/linux/mouse_impl.cpp
        ${SOURCES_DIR}/platform/linux/keyboard_impl.cpp
        ${SOURCES_DIR}/platform/linux/keys.cpp
        ${SOURCES_DIR}/platform/linux/window_impl.cpp
        ${SOURCES_DIR}/platform/linux/events.cpp
        ${SOURCES_DIR}/platform/linux/screen_impl.cpp
        ${SOURCES_DIR}/platform/linux/glloader_impl.cpp
        ${SOURCES_DIR}/platform/linux/glcontext_impl.cpp
        ${SOURCES_DIR}/platform/linux/vulkan_linux.cpp
        ${SOURCES_DIR}/platform/linux/vk_surface_impl.cpp
        ${SOURCES_DIR}/platform/linux/file_impl.cpp
    )

    set(LIBS_PLATFORM
        X11 
        GLX
        GL
        vulkan
    )

    set(LIBS_DIRS_PLATFORM
        ${THIRDPARTY_DIR}/vulkan-sdk/linux/lib
    )

elseif(STRAITX_PLATFORM_WINDOWS)
    set(SOURCES_PLATFORM_D
       ${SOURCES_DIR}/platform/windows/platform_windows.cpp
       ${SOURCES_DIR}/platform/windows/mouse.cpp
       ${SOURCES_DIR}/platform/windows/keyboard.cpp
       ${SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SOURCES_DIR}/platform/windows/window_win32.cpp
       ${SOURCES_DIR}/platform/windows/events_win32.cpp
       ${SOURCES_DIR}/platform/windows/screen_win32.cpp
    )

    set(LIBS_PLATFORM
        
    )

    set(LIBS_DIRS_PLATFORM
        ${THIRDPARTY_DIR}/vulkan-sdk/windows/lib
    )

else()
    message(FATAL_ERROR "Your platform is not supported yet")
endif()


add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glad)

add_library(StraitX STATIC 
    ${SOURCES_PLATFORM_D} 
    ${SOURCES_PLATFORM_I}
    ${SOURCES_CORE}
    ${SOURCES_GRAPHICS}
    ${SOURCES_SERVERS}
    ${SOURCES_MAIN}
)
target_include_directories(StraitX PUBLIC 
    ${INCLUDES}
)
target_link_libraries(StraitX PUBLIC 
    ${LIBS_PLATFORM}
    ${LIBS_ENGINE}
    ${CMAKE_DL_LIBS}
)
target_link_directories(StraitX PUBLIC
    ${LIBS_DIRS_PLATFORM}
)