cmake_minimum_required(VERSION 3.12)
project(NoerEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (LINUX)
    # Setze Flags speziell für den Debug-Build-Typ
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -g -static-libasan")

    # Optional: Zusätzliche Debugging-Flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
endif()

add_subdirectory(src/functional_base)
add_subdirectory(src/gpu)
add_subdirectory(src/gui)
add_subdirectory(src/input)


include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SDL3)

set(SDLTTF_PLUTOSVG OFF CACHE BOOL "Enable SVG support in FreeType" FORCE)

FetchContent_Declare(SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SDL3_ttf)

if (WIN32)
    set(DAV1D_ASM OFF CACHE BOOL "Disable DAV1D_ASM" FORCE)
endif()
FetchContent_Declare(SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.0
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SDL3_image)


add_library(sdl_all INTERFACE)
target_link_libraries(sdl_all INTERFACE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)


set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "Something something" FORCE)
FetchContent_Declare(SDL3_shadercross
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
    GIT_TAG main
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SDL3_shadercross)

target_link_libraries(sdl_all INTERFACE SDL3_shadercross::SDL3_shadercross)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)
target_compile_definitions(compiler_flags INTERFACE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(noer_engine STATIC
    src/Engine.cpp
    src/Engine.hpp
)

target_include_directories(noer_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(noer_engine PUBLIC functional_base gpu gui input sdl_all compiler_flags)

set(BUILD_NOER_TESTS ON)
if(BUILD_NOER_TESTS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    
    add_executable(noer_engine_tests src/main.cpp)

    target_link_libraries(noer_engine_tests PUBLIC noer_engine compiler_flags)

    add_custom_command(
        TARGET noer_engine_tests
        COMMENT "Copy shaders folder"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:noer_engine_tests>/resources/shaders
    )
endif()