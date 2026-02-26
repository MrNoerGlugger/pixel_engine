#include "Rectangle.hpp"


void Rectangle::set_color(SDL_Color color) {
    this->color = color;
}

void Rectangle::set_size(float x, float y) {
    this->size.x = x;
    this->size.y = y;
    init();
}


void Rectangle::create_graphics_pipeline() {
    string pipelineId = "rectangle";
    this->pipeline_id = pipelineId;

    if (graphics::graphics_pipeline_map.count(pipelineId) != 0) {
        this->pipeline = graphics::graphics_pipeline_map.at(pipeline_id);
        return;
    }

	// Create the shaders
	SDL_GPUShader* vertexShader = game::load_shader(graphics::device, "ColoredRectangle.vert", 0, 1, 0, 0);
	if (vertexShader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
		return;
	}

	SDL_GPUShader* fragmentShader = game::load_shader(graphics::device, "SolidColor.frag", 0, 0, 0, 0);
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
    pipelineCreateInfo.target_info.color_target_descriptions = gpuColorTargetDescription;
	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    SDL_GPUVertexBufferDescription vertexBufferDescription[1] =
    { SDL_GPUVertexBufferDescription(
        0,
        sizeof(Uint32),
        SDL_GPU_VERTEXINPUTRATE_VERTEX,
        0
        )
    };
    SDL_GPUVertexAttribute vertexAttribute[1] =
    { SDL_GPUVertexAttribute(
        0,
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_UINT,
        0
        )
    };

    pipelineCreateInfo.vertex_input_state = SDL_GPUVertexInputState(
        &vertexBufferDescription[0],
        1,
        &vertexAttribute[0],
        1
    ); 

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

void Rectangle::init() {
    create_graphics_pipeline();
    viewport = { (float)position.x, (float)position.y, (float)size.x, (float)size.y, 0.0f, 1.0f };
}

void Rectangle::draw() {
    graphics::bind_graphics_pipeline(pipeline, pipeline_id);

    SDL_GPUViewport scaled_viewport = viewport;
    scaled_viewport.x *= graphics::zoom[0];
    scaled_viewport.w *= graphics::zoom[0];
    scaled_viewport.y *= graphics::zoom[1];
    scaled_viewport.h *= graphics::zoom[1];
    SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);

    float normalized_color[4]{color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
    SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &normalized_color, sizeof(normalized_color));

    SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);
}

void Rectangle::free() {
    // Logger::log_debug("Rectangle freed");
}