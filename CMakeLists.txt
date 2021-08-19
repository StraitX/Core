cmake_minimum_required(VERSION 3.10)

project(
    StraitXEngine 
    DESCRIPTION "StraitXEngine project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

include(Config)

set(SOURCES_CORE
    ${SOURCES_DIR}/core/assert.cpp
	${SOURCES_DIR}/core/basic_types_printer.cpp
	${SOURCES_DIR}/core/print.cpp
    ${SOURCES_DIR}/core/result.cpp
    ${SOURCES_DIR}/core/string.cpp

    ${SOURCES_DIR}/core/allocators/linear_allocator.cpp

    ${SOURCES_DIR}/core/os/glloader.cpp
    ${SOURCES_DIR}/core/os/keyboard.cpp
    ${SOURCES_DIR}/core/os/memory.cpp
)

set(SOURCES_GRAPHICS_API
    ${SOURCES_DIR}/graphics/api/graphics_api.cpp
    ${SOURCES_DIR}/graphics/api/gpu.cpp
    ${SOURCES_DIR}/graphics/api/texture.cpp
    ${SOURCES_DIR}/graphics/api/semaphore.cpp
    ${SOURCES_DIR}/graphics/api/swapchain.cpp
    ${SOURCES_DIR}/graphics/api/fence.cpp
)

set(SOURCES_GRAPHICS_API_VULKAN
    ${SOURCES_DIR}/graphics/api/vulkan/debug.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/graphics_api_impl.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/gpu_impl.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/queue.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/semaphore_impl.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/swapchain_impl.cpp
    ${SOURCES_DIR}/graphics/api/vulkan/fence_impl.cpp
)

set(SOURCES_GRAPHICS
    ${SOURCES_DIR}/graphics/color.cpp
    ${SOURCES_DIR}/graphics/image_loader.cpp
    ${SOURCES_DIR}/graphics/image.cpp
)

set(SOURCES_MAIN
    ${SOURCES_DIR}/main/engine.cpp
    ${SOURCES_DIR}/main/application.cpp
    ${SOURCES_DIR}/main/runtime.cpp
)

if(STRAITX_PLATFORM_LINUX)

    set(SOURCES_PLATFORM
		${SOURCES_DIR}/platform/linux/entry_point.cpp

        ${SOURCES_DIR}/platform/linux/display_server.cpp
        ${SOURCES_DIR}/platform/linux/mouse_impl.cpp
        ${SOURCES_DIR}/platform/linux/keyboard_impl.cpp
        ${SOURCES_DIR}/platform/linux/keys.cpp
        ${SOURCES_DIR}/platform/linux/window_impl.cpp
        ${SOURCES_DIR}/platform/linux/screen_impl.cpp
        ${SOURCES_DIR}/platform/linux/glloader_impl.cpp
        ${SOURCES_DIR}/platform/linux/opengl_context_impl.cpp
        ${SOURCES_DIR}/platform/linux/vulkan_linux.cpp
        ${SOURCES_DIR}/platform/linux/vulkan_surface_impl.cpp
        ${SOURCES_DIR}/platform/linux/clock_impl.cpp
        ${SOURCES_DIR}/platform/unix/sleep_impl.cpp
		${SOURCES_DIR}/platform/unix/memory_impl.cpp
        ${SOURCES_DIR}/platform/unix/file_impl.cpp
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
    set(SOURCES_PLATFORM
       ${SOURCES_DIR}/platform/windows/entry_point.cpp

       ${SOURCES_DIR}/platform/windows/mouse_impl.cpp
       ${SOURCES_DIR}/platform/windows/keyboard_impl.cpp
       ${SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SOURCES_DIR}/platform/windows/window_impl.cpp
       ${SOURCES_DIR}/platform/windows/events.cpp
       ${SOURCES_DIR}/platform/windows/glloader_impl.cpp
       ${SOURCES_DIR}/platform/windows/wgl.cpp
       ${SOURCES_DIR}/platform/windows/opengl_context_impl.cpp
       ${SOURCES_DIR}/platform/windows/vulkan_windows.cpp
       ${SOURCES_DIR}/platform/windows/vulkan_surface_impl.cpp
       ${SOURCES_DIR}/platform/windows/clock_impl.cpp
       ${SOURCES_DIR}/platform/windows/sleep_impl.cpp
       ${SOURCES_DIR}/platform/windows/file_impl.cpp
       ${SOURCES_DIR}/platform/windows/memory_impl.cpp
    )

    set(LIBS_PLATFORM
        opengl32
        vulkan-1
        winmm
    )

    set(LIBS_DIRS_PLATFORM
        ${THIRDPARTY_DIR}/vulkan-sdk/windows/lib
    )

elseif(STRAITX_PLATFORM_MACOS)
    set(SOURCES_PLATFORM
		${SOURCES_DIR}/platform/macos/entry_point.mm

        ${SOURCES_DIR}/platform/macos/sx_application.mm
        ${SOURCES_DIR}/platform/macos/keys.cpp
        ${SOURCES_DIR}/platform/macos/keyboard_impl.cpp
        ${SOURCES_DIR}/platform/macos/mouse_impl.mm
        ${SOURCES_DIR}/platform/macos/sx_view.mm
        ${SOURCES_DIR}/platform/macos/sx_window.mm
        ${SOURCES_DIR}/platform/macos/sx_window_delegate.mm
        ${SOURCES_DIR}/platform/macos/window_impl.mm
        ${SOURCES_DIR}/platform/macos/glloader_impl.mm
        ${SOURCES_DIR}/platform/macos/opengl_context_impl.mm
        ${SOURCES_DIR}/platform/macos/clock_impl.cpp
        ${SOURCES_DIR}/platform/macos/input_manager.cpp
        ${SOURCES_DIR}/platform/unix/sleep_impl.cpp
        ${SOURCES_DIR}/platform/unix/file_impl.cpp
        ${SOURCES_DIR}/platform/unix/memory_impl.cpp
    )

    set(LIBS_PLATFORM
        "-framework Cocoa"
        "-framework AppKit"
        "-framework CoreData"
        "-framework Foundation"
    )

    set(LIBS_DIRS_PLATFORM

    )

else()
    message(FATAL_ERROR "Your platform is not supported yet")
endif()


add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/stb_image)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/imgui)

if(STRAITX_SUPPORT_VULKAN)
	add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glsl2spv)
endif()


set(INCLUDES
    ${SOURCES_DIR}
    ${THIRDPARTY_DIR}/glad/include
    ${THIRDPARTY_DIR}/vulkan-sdk/include
    ${THIRDPARTY_DIR}/stb_image
    ${THIRDPARTY_DIR}/glsl2spv
    ${THIRDPARTY_DIR}/imgui/include
)

set(LIBS_ENGINE
    glad
    stb_image
	imgui
)
if(STRAITX_SUPPORT_VULKAN)
	list(APPEND LIBS_ENGINE glsl2spv)
endif()

add_library(StraitX STATIC 
    ${SOURCES_PLATFORM} 
    ${SOURCES_CORE}
    ${SOURCES_GRAPHICS}
    ${SOURCES_GRAPHICS_API}
    ${SOURCES_GRAPHICS_API_VULKAN}
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