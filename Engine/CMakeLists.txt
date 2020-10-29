cmake_minimum_required(VERSION 3.10)


set(VERSION_MAJOR 0)
set(VERSION_MINOR 00)
set(VERSION_PATCH 10)

project(
    StraitXEngine 
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} 
    DESCRIPTION "StraitXEngine project"
    LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/sources)

include(Config)

set(SOURCES
    ${SOURCES_DIR}/core/assert.cpp
    ${SOURCES_DIR}/main/engine.cpp
    ${SOURCES_DIR}/main/main.cpp
)

set(INCLUDES 
    ${SOURCES_DIR}
)

if(STRAITX_PLATFORM_LINUX)

    set(PLATFORM_SOURCES
        ${SOURCES_DIR}/platform/linux/platform.cpp
        ${SOURCES_DIR}/platform/linux/display.cpp
    )

    set(PLATFORM_LIBS
        X11
    )

else()
    message(FATAL_ERROR "Your platform is not supported yet")
endif()



add_library(StraitX STATIC ${SOURCES} ${PLATFORM_SOURCES})
target_include_directories(StraitX PUBLIC ${INCLUDES})
target_link_libraries(StraitX PUBLIC ${PLATFORM_LIBS})