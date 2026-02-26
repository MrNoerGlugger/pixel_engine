#include "ShaderMap.hpp"

void ShaderMap::create_graphics_pipeline() {
    string pipelineId = "shader";
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
    gpuColorTargetDescription[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
    gpuColorTargetDescription[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
    gpuColorTargetDescription[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    gpuColorTargetDescription[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR;
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

void ShaderMap::init() {
    Texture::init();
    create_graphics_pipeline();
}

void ShaderMap::draw() {
    Texture::draw();
}

void ShaderMap::free() {
    // Logger::log_debug("ShaderMap freed");
}