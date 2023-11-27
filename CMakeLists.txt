cmake_minimum_required(VERSION 3.10)

project(
    StraitXCore
    DESCRIPTION "StraitXCore package"
    LANGUAGES CXX
)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SX_CORE_SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)
set(SX_CORE_THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

include(config/cmake/Config.cmake)

set(SX_CORE_SOURCES_CORE
    ${SX_CORE_SOURCES_DIR}/core/assert.cpp
	${SX_CORE_SOURCES_DIR}/core/basic_types_printer.cpp
	${SX_CORE_SOURCES_DIR}/core/print.cpp
    ${SX_CORE_SOURCES_DIR}/core/result.cpp
    ${SX_CORE_SOURCES_DIR}/core/string.cpp
    ${SX_CORE_SOURCES_DIR}/core/unicode.cpp

    ${SX_CORE_SOURCES_DIR}/core/allocators/linear_allocator.cpp

    ${SX_CORE_SOURCES_DIR}/core/os/keyboard.cpp
    ${SX_CORE_SOURCES_DIR}/core/os/memory.cpp
    ${SX_CORE_SOURCES_DIR}/core/os/file.cpp
    ${SX_CORE_SOURCES_DIR}/core/os/date_time.cpp

    ${SX_CORE_SOURCES_DIR}/core/net/ip.cpp
    ${SX_CORE_SOURCES_DIR}/core/net/tcp_listener.cpp
    ${SX_CORE_SOURCES_DIR}/core/net/tcp_socket.cpp
    ${SX_CORE_SOURCES_DIR}/core/net/udp_socket.cpp
)

if(STRAITX_PLATFORM_LINUX)

    set(SX_CORE_SOURCES_PLATFORM
        ${SX_CORE_SOURCES_DIR}/platform/linux/display_server.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/mouse_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/keyboard_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/keys.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/window_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/screen_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/clock_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/linux/stacktrace_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/unix/string_writer_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/unix/sleep_impl.cpp
		${SX_CORE_SOURCES_DIR}/platform/unix/memory_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/unix/file_impl.cpp
    )

    set(SX_CORE_LIBS_PLATFORM
        X11 
    )

    set(SX_CORE_LIBS_DIRS_PLATFORM

    )

    set(SX_CORE_LINKER_FLAGS_PLATFORM
        "-rdynamic"
    )

elseif(STRAITX_PLATFORM_WINDOWS)
    set(SX_CORE_SOURCES_PLATFORM
       ${SX_CORE_SOURCES_DIR}/platform/windows/mouse_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/keyboard_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/virtual_keys.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/window_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/events.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/clock_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/sleep_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/file_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/memory_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/stacktrace_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/string_writer_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/wchar.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/directory_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/dialog_impl.cpp
       ${SX_CORE_SOURCES_DIR}/platform/windows/net_impl.cpp
    )

    set(SX_CORE_LIBS_PLATFORM
        winmm
        DbgHelp
        ws2_32
    )

    set(SX_CORE_LIBS_DIRS_PLATFORM

    )

    set(SX_CORE_LINKER_FLAGS_PLATFORM

    )

elseif(STRAITX_PLATFORM_MACOS)
    set(SX_CORE_SOURCES_PLATFORM
		${SX_CORE_SOURCES_DIR}/platform/macos/entry_point.mm

        ${SX_CORE_SOURCES_DIR}/platform/macos/sx_application.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/keys.cpp
        ${SX_CORE_SOURCES_DIR}/platform/macos/keyboard_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/macos/mouse_impl.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/sx_view.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/sx_window.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/sx_window_delegate.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/window_impl.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/glloader_impl.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/opengl_context_impl.mm
        ${SX_CORE_SOURCES_DIR}/platform/macos/clock_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/macos/input_manager.cpp
        ${SX_CORE_SOURCES_DIR}/platform/macos/vulkan_macos.cpp
        ${SX_CORE_SOURCES_DIR}/platform/macos/vulkan_surface_impl.mm
        ${SX_CORE_SOURCES_DIR}/platform/unix/sleep_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/unix/file_impl.cpp
        ${SX_CORE_SOURCES_DIR}/platform/unix/memory_impl.cpp
    )

    set(SX_CORE_LIBS_PLATFORM

    )

    set(SX_CORE_LIBS_DIRS_PLATFORM

    )

    set(SX_CORE_LINKER_FLAGS_PLATFORM

    )

else()
    message(FATAL_ERROR "Your platform is not supported yet")
endif()

set(SX_CORE_INCLUDES
    ${SX_CORE_SOURCES_DIR}
)

add_library(StraitXCore STATIC 
    ${SX_CORE_SOURCES_PLATFORM} 
    ${SX_CORE_SOURCES_CORE}
)

target_compile_features(StraitXCore PUBLIC cxx_std_17)

target_link_options(StraitXCore
    PUBLIC ${SX_CORE_LINKER_FLAGS_PLATFORM}
)

target_include_directories(StraitXCore
    PUBLIC ${SX_CORE_INCLUDES}
)
target_link_libraries(StraitXCore
    PRIVATE ${SX_CORE_LIBS_PLATFORM}
    PRIVATE ${SX_CORE_LIBS}
    PRIVATE ${CMAKE_DL_LIBS}
)
target_link_directories(StraitXCore
    PUBLIC ${SX_CORE_LIBS_DIRS_PLATFORM}
)