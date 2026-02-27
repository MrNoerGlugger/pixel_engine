#pragma once

#include "SDL3/SDL_gpu.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

using std::cout;
using std::vector;
using std::map;
using std::pair;
using std::string;

#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   270

typedef enum draw_stage {
    DEFAULT,
    SHADOW,
    LIGHT,
    OVERLAY
} DrawStage;

typedef struct lightsource_data {
    SDL_Point origin;
    Uint32 lightsource_color;
} LightsourceData;

typedef struct shader_data {
    SDL_GPUTexture* shader_texture;
    SDL_Point shader_size;
    vector<lightsource_data> lightsources;
} ShaderData;

namespace graphics {
    extern SDL_GPUDevice* device;
    extern SDL_Window* window;
    extern SDL_GPUCommandBuffer* cmdbuf;
    extern SDL_GPURenderPass* render_pass;
    extern string pipeline_id;

    extern string shadow_map_path;
    extern int gpu_target_texture_amount;
    extern string current_screen;

    extern TTF_Font* font;
    extern TTF_TextEngine* text_engine;
    extern SDL_Color* background;
    extern SDL_Color* text_color;
    extern int frameType;

    extern map<string*, string> resource_path_map;
    extern map<string, pair<SDL_GPUTexture*, SDL_Point>> texture_map;
    extern map<string, shader_data> shader_texture_map;
    extern map<string, pair<SDL_GPUTexture*, SDL_Point>> text_map;
    extern map<string, vector<SDL_FRect>> texture_parts_map;
    extern map<string, SDL_GPUSampler*> sampler_map;
    extern map<string, SDL_GPUGraphicsPipeline*> graphics_pipeline_map;

    extern float zoom[2];
    extern float window_scale[2];
    extern float view_position[2];
    extern DrawStage draw_stage;

    extern void bind_graphics_pipeline(SDL_GPUGraphicsPipeline* pipeline, string pipeline_id);
    extern void end_render_pass();
    extern bool check_in_window(SDL_GPUViewport* viewport);
}