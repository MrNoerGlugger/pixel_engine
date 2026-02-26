#include "LightSource.hpp"

void LightSource::set_position(float x, float y) {
    Drawable::set_position(x, y);
    set_origin({origin.x, origin.y});
}
void LightSource::move(float x, float y) {
    Drawable::move(x, y);
    set_origin({origin.x, origin.y});
}

void LightSource::set_origin(SDL_Point origin) {
    this->origin = origin;
    //position of the light source depends on the alpha of the color, the origin, the position, the rotation and the mirroring of the light source
    //it is position - (alpha - 1) (center needs to be subtracted) + origin (center of the light source) rotated & mirrored
    origin_transformed.x = this->origin.x + 0.5f;
    origin_transformed.y = this->origin.y + 0.5f;
    if (rotation != 0.f || mirroring == 3) { 
        const float PI = 3.14159265f;
        float r = std::sqrt(pow(fabs(origin_transformed.x - texture_size.x / 2.f), 2.f) + pow(fabs(origin_transformed.y - texture_size.y / 2.f), 2.f));
        
        //calculate the angle of the starting rotation
        float a = std::sqrt(pow(fabs(texture_size.x / 2.f - origin_transformed.x), 2.f) + pow(fabs(0.f - origin_transformed.y), 2.f));
        float b = texture_size.x / 2.f;
        float c = r;
        Logger::log_debug(std::format("these are the triangle sides a:{} b:{} c:{}", a, b, c));
        if (a == 0.f || b == 0.f || c == 0.f) {
            Logger::log_warning("Tried calculating an angle in a Triangle with a side of length 0");
            return;
        }
        float starting_rotation = acos(((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c))) * 180.f / PI;
        if (texture_size.x / 2.f - origin_transformed.x > 0.f) {
            starting_rotation = 360.f - starting_rotation;
        }

        float alpha = PI * ((rotation + (mirroring == 3 * 180.f) + (360.f - starting_rotation)) / 180.f);
        origin_transformed.x = r * sin(alpha) + texture_size.x / 2.f;
        origin_transformed.y = r * cos(alpha) + texture_size.y / 2.f;
        Logger::log_debug(std::format("this is the starting rotation: {} and this is the alpha: {}", starting_rotation, alpha));
    }
    
    if (mirroring == 1) {
        origin_transformed.y = texture_size.y - origin_transformed.y;
    }
    if (mirroring == 2) {
        origin_transformed.x = texture_size.x - origin_transformed.x;
    }

    Logger::log_debug(std::format("this is the old origin: {}:{} and this is the new one: {}:{}", origin.x, origin.y, origin_transformed.x, origin_transformed.y));

    origin_transformed.x += position.x;
    origin_transformed.y += position.y;

    viewport.x = origin_transformed.x - (color / 0x01000000 - 1);
    viewport.y = origin_transformed.y - (color / 0x01000000 - 1);
}

void LightSource::set_color(Uint32 color) {
    this->color = color;
    //size of the light source depends on the alpha of the color; it is alpha * 2 (for diameter) - 1 (to not double count the center)
    viewport.w = color / 0x01000000 * 2 - 1;
    viewport.h = color / 0x01000000 * 2 - 1;
    //to set the correct position we set the origin again
    set_origin(this->origin);
}

void LightSource::set_texture_size(SDL_Point texture_size) {
    this->texture_size = texture_size;
    //to set the correct position based on rotation & mirroring we set the origin again
    set_origin(this->origin);
}

void LightSource::set_rotation(float rotation) {
    Texture::set_rotation(rotation);
    //to set the correct position we set the origin again
    set_origin(this->origin);
}

void LightSource::set_mirroring(float mirroring) {
    TextureSheet::set_mirroring(mirroring);
    //to set the correct position we set the origin again
    set_origin(this->origin);
}

void LightSource::create_graphics_pipeline() {
    string pipelineId = "light_source";
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

	SDL_GPUShader* fragmentShader = game::load_shader(graphics::device, "LightSource.frag", 1, 1, 0, 0);
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
    gpuColorTargetDescription[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    gpuColorTargetDescription[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
    gpuColorTargetDescription[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
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

void LightSource::init() {
    Texture::init();
    create_graphics_pipeline();
    set_color(color);
}

void LightSource::draw() {
    SDL_GPUViewport scaled_viewport = viewport;
    scaled_viewport.x *= graphics::zoom[0];
    scaled_viewport.w *= graphics::zoom[0] * scale.x;
    scaled_viewport.y *= graphics::zoom[1];
    scaled_viewport.h *= graphics::zoom[1] * scale.y;

    if (!graphics::check_in_window(&scaled_viewport)) {
        return;
    }

    float normalized_color[4]{(color % 0x00000100) / 255.f, (color % 0x00010000 / 0x00000100) / 255.f, (color % 0x01000000 / 0x00010000) / 255.f, (color / 0x01000000) / 255.f};
    // Logger::log_info(std::format("Color: {}, {}, {}, {}", normalized_color[0], normalized_color[1], normalized_color[2], normalized_color[3]));
    SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &normalized_color, sizeof(normalized_color));

    float origin_array[4]{origin_transformed.x, origin_transformed.y, graphics::zoom[0], graphics::zoom[1]};
    // Logger::log_info(std::format("Origin: {}, {}", origin_array[0], origin_array[1]));
    SDL_PushGPUFragmentUniformData(graphics::cmdbuf, 0, &origin_array, sizeof(origin_array));

    // Logger::log_debug(std::format("LightSource Color is: {:x}", color));

    // Texture::draw();

    auto textureData = graphics::get_texture(*image_path, shader_texture);

    SDL_GPUTextureSamplerBinding samplerBinding = SDL_GPUTextureSamplerBinding{ .texture = textureData.first, .sampler = sampler };
    SDL_BindGPUFragmentSamplers(graphics::render_pass, 0, &samplerBinding, 1);

    
    graphics::bind_graphics_pipeline(pipeline, pipeline_id);
    SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);
    // Logger::log_info(std::format("Viewport: {}, {}, {}, {}", viewport.x, viewport.y, viewport.w, viewport.h));

    SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);
}


void LightSource::free() {
    // Logger::log_debug("LightSource freed");
}