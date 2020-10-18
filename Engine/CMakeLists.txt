cmake_minimum_required(VERSION 3.10)


set(VERSION_MAJOR 0)
set(VERSION_MINOR 00)
set(VERSION_PATCH 1)

project(
    StraitXEngine 
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} 
    DESCRIPTION "StraitXEngine project"
    LANGUAGES CXX
)

set(SOURCES

)

set(INCLUDES
    sources/
)


add_library(StraitX STATIC ${SOURCES})
target_include_directories(StraitX PUBLIC ${INCLUDES})