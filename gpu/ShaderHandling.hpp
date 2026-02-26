#pragma once

#include <Logger.hpp>

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>
// #include "../../../build/_deps/sdl3_shadercross-src/include/SDL3_shadercross/SDL_shadercross.h"

namespace game {
//#ifdef linux
    extern bool compile_shader(
        SDL_GPUDevice* device,
        const char* shaderFilename,
        Uint32 gpu_shader_format
    );
//#endif

    extern SDL_GPUShader* load_shader(
        SDL_GPUDevice* device,
        const char* shaderFilename,
        Uint32 samplerCount,
        Uint32 uniformBufferCount,
        Uint32 storageBufferCount,
        Uint32 storageTextureCount
    );
}