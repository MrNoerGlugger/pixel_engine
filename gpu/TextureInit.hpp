#pragma once

#include <vector>

#include <GPUAccessables.hpp>
#include <Logger.hpp>
#include <ResourceProvider.hpp>

#include <SDL3_image/SDL_image.h>

using std::vector;

namespace graphics {
    extern void register_texture(string path);
    extern void register_shader_texture(string path);
    extern string register_target_texture(int width, int height, string target = "");
    extern pair<SDL_GPUTexture*, SDL_Point> get_texture(string path, bool shader);
    extern ShaderData* get_shader_data(string path);
}