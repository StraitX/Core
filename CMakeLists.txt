cmake_minimum_required(VERSION 3.10)

project(
    StraitXBase
    DESCRIPTION "StraitXBase package"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SX_BASE_SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(SX_BASE_THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

include(Config)

set(SX_BASE_SOURCES_CORE
    ${SX_BASE_SOURCES_DIR}/core/assert.cpp
	${SX_BASE_SOURCES_DIR}/core/basic_types_printer.cpp
	${SX_BASE_SOURCES_DIR}/core/print.cpp
    ${SX_BASE_SOURCES_DIR}/core/result.cpp
    ${SX_BASE_SOURCES_DIR}/core/string.cpp
    ${SX_BASE_SOURCES_DIR}/core/unicode.cpp

    ${SX_BASE_SOURCES_DIR}/core/allocators/linear_allocator.cpp

    ${SX_BASE_SOURCES_DIR}/core/os/glloader.cpp
    ${SX_BASE_SOURCES_DIR}/core/os/keyboard.cpp
    ${SX_BASE_SOURCES_DIR}/core/os/memory.cpp
)

set(SX_BASE_SOURCES_GRAPHICS_API
    ${SX_BASE_SOURCES_DIR}/graphics/api/graphics_api.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/gpu.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/texture.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/semaphore.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/swapchain.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/fence.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/command_buffer.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/render_pass.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/framebuffer.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/shader.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/graphics_pipeline.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/compute_pipeline.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/buffer.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/sampler.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/descriptor_set.cpp
)

set(SX_BASE_SOURCES_GRAPHICS_API_VULKAN
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/debug.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/graphics_api_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/gpu_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/queue.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/semaphore_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/swapchain_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/fence_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/command_buffer_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/texture_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/render_pass_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/framebuffer_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/shader_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/pipeline.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/graphics_pipeline_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/compute_pipeline_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/memory.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/memory_allocator.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/buffer_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/immediate.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/sampler_impl.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/api/vulkan/descriptor_set_impl.cpp
)

set(SX_BASE_SOURCES_GRAPHICS
    ${SX_BASE_SOURCES_DIR}/graphics/color.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/image_loader.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/image.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/render_window.cpp
    ${SX_BASE_SOURCES_DIR}/graphics/render_texture.cpp
)

set(SX_BASE_SOURCES_MAIN
    ${SX_BASE_SOURCES_DIR}/main/guarded_main.cpp
)

if(STRAITX_PLATFORM_LINUX)

    set(SX_BASE_SOURCES_PLATFORM
		${SX_BASE_SOURCES_DIR}/platform/linux/entry_point.cpp

        ${SX_BASE_SOURCES_DIR}/platform/linux/display_server.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/mouse_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/keyboard_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/keys.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/window_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/screen_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/glloader_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/opengl_context_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/vulkan_linux.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/vulkan_surface_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/clock_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/linux/stacktrace_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/unix/sleep_impl.cpp
		${SX_BASE_SOURCES_DIR}/platform/unix/memory_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/unix/file_impl.cpp
    )

    set(SX_BASE_LIBS_PLATFORM
        X11 
        GLX
        GL
        vulkan
    )

    set(SX_BASE_LIBS_DIRS_PLATFORM
        ${SX_BASE_THIRDPARTY_DIR}/vulkan-sdk/linux/lib
    )

    set(SX_BASE_LINKER_FLAGS_PLATFORM
        "-rdynamic"
    )

elseif(STRAITX_PLATFORM_WINDOWS)
    set(SX_BASE_SOURCES_PLATFORM
       ${SX_BASE_SOURCES_DIR}/platform/windows/entry_point.cpp

       ${SX_BASE_SOURCES_DIR}/platform/windows/mouse_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/keyboard_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/window_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/events.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/glloader_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/wgl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/opengl_context_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/vulkan_windows.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/vulkan_surface_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/clock_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/sleep_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/file_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/memory_impl.cpp
       ${SX_BASE_SOURCES_DIR}/platform/windows/stacktrace_impl.cpp
    )

    set(SX_BASE_LIBS_PLATFORM
        opengl32
        vulkan-1
        winmm
        DbgHelp
    )

    set(SX_BASE_LIBS_DIRS_PLATFORM
        ${SX_BASE_THIRDPARTY_DIR}/vulkan-sdk/windows/lib
    )

    set(SX_BASE_LINKER_FLAGS_PLATFORM

    )

elseif(STRAITX_PLATFORM_MACOS)
    set(SX_BASE_SOURCES_PLATFORM
		${SX_BASE_SOURCES_DIR}/platform/macos/entry_point.mm

        ${SX_BASE_SOURCES_DIR}/platform/macos/sx_application.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/keys.cpp
        ${SX_BASE_SOURCES_DIR}/platform/macos/keyboard_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/macos/mouse_impl.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/sx_view.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/sx_window.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/sx_window_delegate.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/window_impl.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/glloader_impl.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/opengl_context_impl.mm
        ${SX_BASE_SOURCES_DIR}/platform/macos/clock_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/macos/input_manager.cpp
        ${SX_BASE_SOURCES_DIR}/platform/unix/sleep_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/unix/file_impl.cpp
        ${SX_BASE_SOURCES_DIR}/platform/unix/memory_impl.cpp
    )

    set(SX_BASE_LIBS_PLATFORM
        "-framework Cocoa"
        "-framework AppKit"
        "-framework CoreData"
        "-framework Foundation"
    )

    set(SX_BASE_LIBS_DIRS_PLATFORM

    )

    set(SX_BASE_LINKER_FLAGS_PLATFORM

    )

else()
    message(FATAL_ERROR "Your platform is not supported yet")
endif()


add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/stb_image)

if(STRAITX_SUPPORT_VULKAN)
	add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glsl2spv)
endif()


set(SX_BASE_INCLUDES
    ${SX_BASE_SOURCES_DIR}
    ${SX_BASE_THIRDPARTY_DIR}/glad/include
    ${SX_BASE_THIRDPARTY_DIR}/vulkan-sdk/include
    ${SX_BASE_THIRDPARTY_DIR}/stb_image
    ${SX_BASE_THIRDPARTY_DIR}/glsl2spv
)

set(SX_BASE_LIBS
    glad
    stb_image
)
if(STRAITX_SUPPORT_VULKAN)
	list(APPEND SX_BASE_LIBS glsl2spv)
endif()

add_library(StraitXBase STATIC 
    ${SX_BASE_SOURCES_PLATFORM} 
    ${SX_BASE_SOURCES_CORE}
    ${SX_BASE_SOURCES_GRAPHICS}
    ${SX_BASE_SOURCES_GRAPHICS_API}
    ${SX_BASE_SOURCES_GRAPHICS_API_VULKAN}
    ${SX_BASE_SOURCES_MAIN}
)

target_link_options(StraitXBase 
    PUBLIC ${SX_BASE_LINKER_FLAGS_PLATFORM}
)

target_include_directories(StraitXBase 
    PUBLIC ${SX_BASE_INCLUDES}
)
target_link_libraries(StraitXBase 
    PRIVATE ${SX_BASE_LIBS_PLATFORM}
    PRIVATE ${SX_BASE_LIBS}
    PRIVATE ${CMAKE_DL_LIBS}
)
target_link_directories(StraitXBase 
    PUBLIC ${SX_BASE_LIBS_DIRS_PLATFORM}
)