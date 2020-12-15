cmake_minimum_required(VERSION 3.10)


set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)
set(VERSION_PATCH 05)

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
    ${SOURCES_DIR}/platform/window.cpp
    ${SOURCES_DIR}/platform/screen.cpp
    ${SOURCES_DIR}/platform/display.cpp
    ${SOURCES_DIR}/platform/error.cpp
    ${SOURCES_DIR}/platform/glloader.cpp
    ${SOURCES_DIR}/platform/glcontext.cpp
)

set(SOURCES_CORE
    ${SOURCES_DIR}/core/assert.cpp
    ${SOURCES_DIR}/core/time_quant.cpp
    ${SOURCES_DIR}/core/memory/stack_allocator.cpp
)

set(SOURCES_GRAPHICS_VULKAN
    ${SOURCES_DIR}/graphics/vulkan/instance.cpp
)

set(SOURCES_GRAPHICS 
    ${SOURCES_GRAPHICS_VULKAN}
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
)

if(STRAITX_PLATFORM_LINUX)

    set(SOURCES_PLATFORM_D
        ${SOURCES_DIR}/platform/linux/platform.cpp
        ${SOURCES_DIR}/platform/linux/display_x11.cpp
        ${SOURCES_DIR}/platform/linux/mouse_x11.cpp
        ${SOURCES_DIR}/platform/linux/keyboard_x11.cpp
        ${SOURCES_DIR}/platform/linux/keys_x11.cpp
        ${SOURCES_DIR}/platform/linux/window_x11.cpp
        ${SOURCES_DIR}/platform/linux/events_x11.cpp
        ${SOURCES_DIR}/platform/linux/screen_x11.cpp
        ${SOURCES_DIR}/platform/linux/glloader_x11.cpp
        ${SOURCES_DIR}/platform/linux/glcontext_x11.cpp
    )

    set(LIBS_PLATFORM
        X11 
        GLX
        GL
        vulkan
    )
elseif(STRAITX_PLATFORM_WINDOWS)
    set(SOURCES_PLATFORM_D
       ${SOURCES_DIR}/platform/windows/platform.cpp
	   ${SOURCES_DIR}/platform/windows/hinstance.cpp
       ${SOURCES_DIR}/platform/windows/mouse.cpp
       ${SOURCES_DIR}/platform/windows/keyboard.cpp
       ${SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SOURCES_DIR}/platform/windows/window_win32.cpp
       ${SOURCES_DIR}/platform/windows/events_win32.cpp
       ${SOURCES_DIR}/platform/windows/screen_win32.cpp
    )

    set(LIBS_PLATFORM

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
target_include_directories(StraitX PUBLIC ${INCLUDES})
target_link_libraries(StraitX PUBLIC 
    ${LIBS_PLATFORM}
    ${LIBS_ENGINE}
    ${CMAKE_DL_LIBS}
)