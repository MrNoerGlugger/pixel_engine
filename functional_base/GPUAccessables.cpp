#include "GPUAccessables.hpp"
#include "SDL3/SDL_gpu.h"

SDL_GPUDevice* graphics::device;
SDL_Window* graphics::window;
SDL_GPUCommandBuffer* graphics::cmdbuf;
SDL_GPURenderPass* graphics::render_pass;
string graphics::pipeline_id = "";

string graphics::shadow_map_path = "";
int graphics::gpu_target_texture_amount = 0;
string* graphics::current_screen = nullptr;

TTF_Font* graphics::font;
TTF_TextEngine* graphics::text_engine;
SDL_Color* graphics::background;
SDL_Color* graphics::text_color;
int graphics::frameType = 0;

map<string*, string> graphics::resource_path_map;
map<string, pair<SDL_GPUTexture*, SDL_Point>> graphics::texture_map;
map<string, shader_data> graphics::shader_texture_map;
map<string, pair<SDL_GPUTexture*, SDL_Point>> graphics::text_map;
map<string, vector<SDL_FRect>> graphics::texture_parts_map;
map<string, SDL_GPUSampler*> graphics::sampler_map;
map<string, SDL_GPUGraphicsPipeline*> graphics::graphics_pipeline_map;

float graphics::zoom[2]{1.0f, 1.0f};
float graphics::window_scale[2]{1.0f, 1.0f};
float graphics::view_position[2]{0.f, 0.f};
DrawStage graphics::draw_stage = SHADOW;

void graphics::bind_graphics_pipeline(SDL_GPUGraphicsPipeline* pipeline, string pipeline_id) {
    if (graphics::pipeline_id == pipeline_id) {
        return;
    }
    graphics::pipeline_id = pipeline_id;
    SDL_BindGPUGraphicsPipeline(graphics::render_pass, pipeline);
}

void graphics::end_render_pass() {
    graphics::pipeline_id = "";
    SDL_EndGPURenderPass(graphics::render_pass);
}

bool graphics::check_in_window(SDL_GPUViewport* viewport) {
    SDL_Rect window_rect = {(int)(-view_position[0]), (int)(-view_position[1]), (int)(SCREEN_WIDTH * window_scale[0]), (int)(SCREEN_HEIGHT * window_scale[1])};
    SDL_Rect viewport_rect = {(int)(viewport->x), (int)(viewport->y), (int)(viewport->w), (int)(viewport->h)};
    return SDL_HasRectIntersection(&window_rect, &viewport_rect);
}