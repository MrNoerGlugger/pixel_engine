#include "Texture.hpp"
#include "../../../init/Textures.hpp"

Texture::Texture() {
    image_path = &textures::menu;
}

Texture::Texture(string* image_path) {
    this->image_path = image_path;
}

void Texture::set_rotation(float rotation) {
    this->rotation = rotation;
}
void Texture::set_image_path(string* image_path, bool shader_texture) {
    this->image_path = image_path;
    this->shader_texture = shader_texture;
    geometry_update_needed = true;
    this->init();
}

void Texture::create_graphics_pipeline() {
    string pipelineId = "texture";
    this->pipeline_id = pipelineId;

    if (graphics::graphics_pipeline_map.count(pipelineId) != 0) {
        this->pipeline = graphics::graphics_pipeline_map.at(pipeline_id);
        return;
    }

    // Create the shaders
	SDL_GPUShader* vertexShader = game::load_shader(graphics::device, "Texture.vert", 0, 1, 0, 0);
	if (vertexShader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
		return;
	}
	SDL_GPUShader* fragmentShader = game::load_shader(graphics::device, "TexturedQuad.frag", 1, 0, 0, 0);
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

SDL_GPUSampler* createGPUSampler(SDL_GPUSampler* sampler) {
    string samplerId = "nearest";

    if (graphics::sampler_map.count(samplerId) != 0) {
        return graphics::sampler_map.at(samplerId);
    }

    SDL_GPUSamplerCreateInfo samplerCreateInfo;
    samplerCreateInfo.min_filter = SDL_GPU_FILTER_NEAREST;
    samplerCreateInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
    samplerCreateInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    samplerCreateInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerCreateInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerCreateInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerCreateInfo.compare_op = SDL_GPU_COMPAREOP_EQUAL;

    sampler = SDL_CreateGPUSampler(graphics::device, &samplerCreateInfo);

    graphics::sampler_map.insert(pair<string, SDL_GPUSampler*>(samplerId, sampler));
    return graphics::sampler_map.at(samplerId);
}

void Texture::update_geometry() {
    if (!geometry_update_needed) {
        return;
    }
    geometry_update_needed = false;

    auto textureData = graphics::get_texture(*image_path, shader_texture);

    viewport = { position.x, position.y, (float)textureData.second.x, (float)textureData.second.y, 0.0f, 1.0f };
}

void Texture::init() {
    create_graphics_pipeline();
    SDL_GPUSampler* sampler_temp = NULL;
    sampler = createGPUSampler(sampler_temp);
    update_geometry();
}

void Texture::draw() {
    auto textureData = graphics::get_texture(*image_path, shader_texture);
    SDL_GPUTextureSamplerBinding samplerBinding = SDL_GPUTextureSamplerBinding{ .texture = textureData.first, .sampler = sampler };
    SDL_BindGPUFragmentSamplers(graphics::render_pass, 0, &samplerBinding, 1);

    graphics::bind_graphics_pipeline(pipeline, pipeline_id);

    SDL_GPUViewport scaled_viewport = viewport;
    scaled_viewport.x *= graphics::zoom[0];
    scaled_viewport.w *= graphics::zoom[0] * scale.x;
    scaled_viewport.y *= graphics::zoom[1];
    scaled_viewport.h *= graphics::zoom[1] * scale.y;

    SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);

    SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &rotation, sizeof(rotation));

    SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);
}

void Texture::free() {
    // Logger::log_debug("Texture freed");
}