cmake_minimum_required(VERSION 3.10)


set(VERSION_MAJOR 0)
set(VERSION_MINOR 02)
set(VERSION_PATCH 07)

project(
    StraitXEngine 
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} 
    DESCRIPTION "StraitXEngine project"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

include(Config)

set(SOURCES_PLATFORM_I
    ${SOURCES_DIR}/platform/memory.cpp
    ${SOURCES_DIR}/platform/result.cpp
    ${SOURCES_DIR}/platform/glloader.cpp
    ${SOURCES_DIR}/platform/keyboard.cpp
)

set(SOURCES_CORE
    ${SOURCES_DIR}/core/assert.cpp
    ${SOURCES_DIR}/core/memory/linear_allocator.cpp
    ${SOURCES_DIR}/core/string.cpp
	${SOURCES_DIR}/core/print.cpp
	${SOURCES_DIR}/core/basic_types_printer.cpp
	${SOURCES_DIR}/core/subsystem.cpp
)

set(SOURCES_GRAPHICS_VULKAN
    ${SOURCES_DIR}/graphics/vulkan/debug.cpp

	${SOURCES_DIR}/graphics/vulkan/graphics_context_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/queue.cpp
    ${SOURCES_DIR}/graphics/vulkan/gpu.cpp
    ${SOURCES_DIR}/graphics/vulkan/memory.cpp
    ${SOURCES_DIR}/graphics/vulkan/fence.cpp
    ${SOURCES_DIR}/graphics/vulkan/semaphore.cpp
    ${SOURCES_DIR}/graphics/vulkan/memory_allocator.cpp
    ${SOURCES_DIR}/graphics/vulkan/gpu_buffer_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/command_buffer.cpp
    ${SOURCES_DIR}/graphics/vulkan/swapchain.cpp
    ${SOURCES_DIR}/graphics/vulkan/cpu_buffer_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/texture_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/sampler_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/shader_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/render_pass_impl.cpp
	${SOURCES_DIR}/graphics/vulkan/descriptor_set_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/graphics_pipeline_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/framebuffer_impl.cpp
    ${SOURCES_DIR}/graphics/vulkan/dma_impl.cpp

    ${SOURCES_DIR}/graphics/vulkan/graphics_api_vtable.cpp
)

set(SOURCES_GRAPHICS_OPENGL
    ${SOURCES_DIR}/graphics/opengl/graphics_context_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/gpu_buffer_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/cpu_buffer_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/texture_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/sampler_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/shader_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/render_pass_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/descriptor_set_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/graphics_pipeline_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/framebuffer_impl.cpp
    ${SOURCES_DIR}/graphics/opengl/dma_impl.cpp

    ${SOURCES_DIR}/graphics/opengl/graphics_api_vtable.cpp
)

set(SOURCES_GRAPHICS_API
    ${SOURCES_DIR}/graphics/api/format.cpp
    ${SOURCES_DIR}/graphics/api/graphics_api_loader.cpp
	${SOURCES_DIR}/graphics/api/graphics_context.cpp
	${SOURCES_DIR}/graphics/api/gpu_command_buffer.cpp
    ${SOURCES_DIR}/graphics/api/gpu_configuration.cpp
    ${SOURCES_DIR}/graphics/api/gpu_buffer.cpp
    ${SOURCES_DIR}/graphics/api/cpu_buffer.cpp
    ${SOURCES_DIR}/graphics/api/texture.cpp
    ${SOURCES_DIR}/graphics/api/sampler.cpp
    ${SOURCES_DIR}/graphics/api/shader.cpp
    ${SOURCES_DIR}/graphics/api/render_pass.cpp
    ${SOURCES_DIR}/graphics/api/descriptor_set.cpp
    ${SOURCES_DIR}/graphics/api/graphics_pipeline.cpp
    ${SOURCES_DIR}/graphics/api/framebuffer.cpp
    ${SOURCES_DIR}/graphics/api/dma.cpp
)

if(STRAITX_SUPPORT_VULKAN)
	message("Vulkan")

	list(APPEND SOURCES_GRAPHICS_API_IMPL ${SOURCES_GRAPHICS_VULKAN})
endif()

if(STRAITX_SUPPORT_OPENGL)
    message("OpenGL")

    list(APPEND SOURCES_GRAPHICS_API_IMPL ${SOURCES_GRAPHICS_OPENGL})
endif()

set(SOURCES_GRAPHICS
	${SOURCES_GRAPHICS_API_IMPL}
    ${SOURCES_GRAPHICS_API}
    ${SOURCES_DIR}/graphics/color.cpp
    ${SOURCES_DIR}/graphics/image_loader.cpp
    ${SOURCES_DIR}/graphics/image.cpp
    ${SOURCES_DIR}/graphics/batch_renderer_2d.cpp
	${SOURCES_DIR}/graphics/imgui/imgui_backend.cpp
	${SOURCES_DIR}/graphics/renderer_2d.cpp
)

set(SOURCES_MAIN
    ${SOURCES_DIR}/main/engine.cpp
    ${SOURCES_DIR}/main/application.cpp
    ${SOURCES_DIR}/main/runtime.cpp
)

if(STRAITX_PLATFORM_LINUX)

    set(SOURCES_PLATFORM_D
		${SOURCES_DIR}/platform/linux/entry_point.cpp

        ${SOURCES_DIR}/platform/linux/window_system_impl.cpp
        ${SOURCES_DIR}/platform/linux/mouse_impl.cpp
        ${SOURCES_DIR}/platform/linux/keyboard_impl.cpp
        ${SOURCES_DIR}/platform/linux/keys.cpp
        ${SOURCES_DIR}/platform/linux/window_impl.cpp
        ${SOURCES_DIR}/platform/linux/events.cpp
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
    set(SOURCES_PLATFORM_D
       ${SOURCES_DIR}/platform/windows/entry_point.cpp

       ${SOURCES_DIR}/platform/windows/mouse_impl.cpp
       ${SOURCES_DIR}/platform/windows/keyboard_impl.cpp
       ${SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SOURCES_DIR}/platform/windows/window_impl.cpp
       ${SOURCES_DIR}/platform/windows/events.cpp
       ${SOURCES_DIR}/platform/windows/window_system_impl.cpp
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
    set(SOURCES_PLATFORM_D
		${SOURCES_DIR}/platform/macos/entry_point.cpp

        ${SOURCES_DIR}/platform/macos/sx_application.mm
        ${SOURCES_DIR}/platform/macos/keys.cpp
        ${SOURCES_DIR}/platform/macos/keyboard_impl.cpp
        ${SOURCES_DIR}/platform/macos/mouse_impl.mm
        ${SOURCES_DIR}/platform/macos/sx_window.mm
        ${SOURCES_DIR}/platform/macos/window_impl.mm
        ${SOURCES_DIR}/platform/macos/window_system_impl.mm
        ${SOURCES_DIR}/platform/macos/glloader_impl.mm
        ${SOURCES_DIR}/platform/macos/ns_opengl_context_wrapper.mm
        ${SOURCES_DIR}/platform/macos/opengl_context_impl.mm
        ${SOURCES_DIR}/platform/macos/clock_impl.cpp
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
    ${SOURCES_PLATFORM_D} 
    ${SOURCES_PLATFORM_I}
    ${SOURCES_CORE}
    ${SOURCES_GRAPHICS}
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