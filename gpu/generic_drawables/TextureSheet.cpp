#include "TextureSheet.hpp"
#include "SDL3/SDL_gpu.h"

TextureSheet::TextureSheet() {}


void TextureSheet::set_part(int part) {
    this->part = part;
}
void TextureSheet::set_mirroring(float mirroring) {
    this->mirroring = mirroring;
}
void TextureSheet::set_color_multiplier(float color_mult) {
    this->color_mult = color_mult;
}
SDL_FRect TextureSheet::get_bounds() {
    return {position.x, position.y, (*parts)[part].w  * scale.x, (*parts)[part].h  * scale.y};
}

void TextureSheet::create_graphics_pipeline() {
    string pipelineId = "texture_sheet";
    this->pipeline_id = pipelineId;

    if (graphics::graphics_pipeline_map.count(pipelineId) != 0) {
        this->pipeline = graphics::graphics_pipeline_map.at(pipeline_id);
        return;
    }

    // Create the shaders
	SDL_GPUShader* vertexShader = game::load_shader(graphics::device, "TextureSheet.vert", 0, 1, 0, 0);
	if (vertexShader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
		return;
	}
	SDL_GPUShader* fragmentShader = game::load_shader(graphics::device, "TexturedMultipliedQuad.frag", 1, 1, 0, 0);
	if (fragmentShader == NULL)
	{
		SDL_Log("Failed to create fragment shader!");
		return;
	}


    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo();
    pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	pipelineCreateInfo.vertex_shader = vertexShader;
	pipelineCreateInfo.fragment_shader = fragmentShader;
    pipelineCreateInfo.target_info.num_color_targets = 1;
    SDL_GPUColorTargetDescription gpuColorTargetDescription[1] = { SDL_GPUColorTargetDescription() };
    gpuColorTargetDescription[0].format = SDL_GetGPUSwapchainTextureFormat(graphics::device, graphics::window);
    gpuColorTargetDescription[0].blend_state = SDL_GPUColorTargetBlendState();
    gpuColorTargetDescription[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    gpuColorTargetDescription[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
    gpuColorTargetDescription[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    gpuColorTargetDescription[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
    gpuColorTargetDescription[0].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    gpuColorTargetDescription[0].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
    gpuColorTargetDescription[0].blend_state.enable_blend = true;
    gpuColorTargetDescription[0].blend_state.enable_color_write_mask = false;
    pipelineCreateInfo.target_info.color_target_descriptions = gpuColorTargetDescription;
	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(graphics::device, &pipelineCreateInfo);
	if (pipeline == NULL) {
		Logger::log_error(std::format("Failed to create pipeline: {}", SDL_GetError()));
		return;
	}

    SDL_ReleaseGPUShader(graphics::device, vertexShader);
    SDL_ReleaseGPUShader(graphics::device, fragmentShader);

    graphics::graphics_pipeline_map.insert(pair<string, SDL_GPUGraphicsPipeline*>(pipelineId, pipeline));
    this->pipeline = graphics::graphics_pipeline_map.at(pipeline_id);
}

void TextureSheet::init() {
    Texture::init();
    create_graphics_pipeline();
    if (image_path == nullptr) {
        Logger::log_warning("TextureSheet image_path has not been set!");
        return;
    }
    if (graphics::texture_parts_map.count(*image_path) == 0) {
        parts = new vector<SDL_FRect>();
        parts->push_back(SDL_FRect{0, 0, viewport.w, viewport.h});
        return;
    }
    parts = &graphics::texture_parts_map.at(*image_path);
}

void TextureSheet::draw() {
    if (image_path == nullptr) {
        Logger::log_warning("TextureSheet image_path has not been set!");
        return;
    }
    auto textureData = graphics::get_texture(*image_path, shader_texture);

    graphics::bind_graphics_pipeline(pipeline, pipeline_id);

    SDL_GPUViewport scaled_viewport = {position.x, position.y, (*parts)[part].w, (*parts)[part].h, 0.0f, 1.0f};
    scaled_viewport.x *= graphics::zoom[0];
    scaled_viewport.w *= graphics::zoom[0] * scale.x;
    scaled_viewport.y *= graphics::zoom[1];
    scaled_viewport.h *= graphics::zoom[1] * scale.y;

    SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);

    SDL_PushGPUFragmentUniformData(graphics::cmdbuf, 0, &color_mult, sizeof(color_mult));
    float part_data[6]{(*parts)[part].x / viewport.w, (*parts)[part].y / viewport.h, ((*parts)[part].x + (*parts)[part].w) / viewport.w, ((*parts)[part].y + (*parts)[part].h) / viewport.h, 
        rotation, mirroring};
    SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &part_data, sizeof(part_data));

    SDL_GPUTextureSamplerBinding samplerBinding = SDL_GPUTextureSamplerBinding{ .texture = textureData.first, .sampler = sampler };
    SDL_BindGPUFragmentSamplers(graphics::render_pass, 0, &samplerBinding, 1);
    SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);
}

void TextureSheet::free() {
    // Logger::log_debug("TextureSheet freed");
}