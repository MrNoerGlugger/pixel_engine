#include "Text.hpp"

Text::Text() {}

Text::Text(string text_string) {
    text = text_string;
}

void Text::set_position(float x, float y) {
    position.x = x;
    position.y = y;
}
SDL_FRect Text::get_bounds() {
    return {position.x, position.y, viewport.w * scale.x, viewport.h * scale.y};
}

void Text::set_text_string(std::string text_string) {
    text = text_string;
}
void Text::set_text_color(SDL_Color text_color) {
    this->text_color = text_color;
}

void Text::create_graphics_pipeline() {
    string pipelineId = "text";
    this->pipeline_id = pipelineId;

    if (graphics::graphics_pipeline_map.count(pipelineId) != 0) {
        this->pipeline = graphics::graphics_pipeline_map.at(pipeline_id);
        return;
    }

    // Create the shaders
	SDL_GPUShader* vertexShader = game::load_shader(graphics::device, "ColoredTexture.vert", 0, 1, 0, 0);
	if (vertexShader == NULL)
	{
		SDL_Log("Failed to create vertex shader!");
		return;
	}
	SDL_GPUShader* fragmentShader = game::load_shader(graphics::device, "TexturedColoredQuad.frag", 1, 0, 0, 0);
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

SDL_GPUSampler* createGPUSampler2(SDL_GPUSampler* sampler) {
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

    sampler = SDL_CreateGPUSampler(graphics::device, &samplerCreateInfo);

    graphics::sampler_map.insert(pair<string, SDL_GPUSampler*>(samplerId, sampler));
    return graphics::sampler_map.at(samplerId);
}

pair<SDL_GPUTexture*, SDL_Point> createGPUText(string& text_string) {
    string identifier = std::format("{}", text_string);

    if (graphics::text_map.count(identifier) != 0) {
        return graphics::text_map.at(identifier);
    }

    TTF_Text* text_temp = TTF_CreateText(graphics::text_engine, graphics::font, text_string.c_str(), text_string.size());

    int x, y;
    TTF_GetTextSize(text_temp, &x, &y);
    SDL_Surface* imageData = SDL_CreateSurface(x, y, SDL_PIXELFORMAT_RGBA32);
    TTF_DrawSurfaceText(text_temp, 0, 0, imageData);
    if (imageData == NULL) 
    {
        SDL_Log("Could not load image data!");
        return graphics::text_map.at(identifier);
    }

    TTF_DestroyText(text_temp);
    
    SDL_GPUTextureCreateInfo textureCreateInfo = SDL_GPUTextureCreateInfo {
        SDL_GPU_TEXTURETYPE_2D,
        SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        SDL_GPU_TEXTUREUSAGE_SAMPLER,
        (Uint32)imageData->w,
        (Uint32)imageData->h,
        1,
        1,
        SDL_GPU_SAMPLECOUNT_1,
        0
    };

    SDL_GPUTexture* texture = SDL_CreateGPUTexture(graphics::device, &textureCreateInfo);

    SDL_Point textureSize = SDL_Point{imageData->w, imageData->h};


    SDL_GPUTransferBufferCreateInfo bufferCreateInfo = SDL_GPUTransferBufferCreateInfo {
        SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        (Uint32)(imageData->w * imageData->h * 4),
        0
    };

    SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(graphics::device, &bufferCreateInfo);

    Uint8* textureTransferPointer = (Uint8*)SDL_MapGPUTransferBuffer(graphics::device, textureTransferBuffer, false);

    SDL_memcpy(textureTransferPointer, imageData->pixels, imageData->w * imageData->h * 4);
    SDL_UnmapGPUTransferBuffer(graphics::device, textureTransferBuffer);


    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(graphics::device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTextureTransferInfo textureTransferInfo = SDL_GPUTextureTransferInfo {
        .transfer_buffer = textureTransferBuffer,
        .offset = 0
    };
    SDL_GPUTextureRegion textureRegion = SDL_GPUTextureRegion {
        .texture = texture,
        .w = (Uint32)imageData->w,
        .h = (Uint32)imageData->h,
        .d = 1
    };

    SDL_UploadToGPUTexture(
        copyPass,
        &textureTransferInfo,
        &textureRegion,
        false
    );

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	SDL_DestroySurface(imageData);
	SDL_ReleaseGPUTransferBuffer(graphics::device, textureTransferBuffer);


    graphics::text_map.insert(std::pair<string, pair<SDL_GPUTexture*, SDL_Point>>(identifier, pair<SDL_GPUTexture*, SDL_Point>(texture, textureSize)));

    return graphics::text_map.at(identifier);
}

void Text::init() {
    create_graphics_pipeline();
    sampler = createGPUSampler2(sampler);
    // createGlyphs(text, glyphs);
    auto textureData = createGPUText(text);
    text_texture = textureData.first;

    viewport = { position.x, position.y, (float)textureData.second.x, (float)textureData.second.y, 0.0f, 1.0f };
}

void Text::draw() {
    graphics::bind_graphics_pipeline(pipeline, pipeline_id);

    SDL_GPUViewport scaled_viewport = viewport;
    scaled_viewport.x *= graphics::zoom[0];
    scaled_viewport.w *= graphics::zoom[0] * scale.x;
    scaled_viewport.y *= graphics::zoom[1];
    scaled_viewport.h *= graphics::zoom[1] * scale.y;

    SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);

    float normalized_color[4]{graphics::text_color->r / 255.f, graphics::text_color->g / 255.f, graphics::text_color->b / 255.f, graphics::text_color->a / 255.f};
    if (text_color.r != 0 || text_color.g != 0 || text_color.b != 0 || text_color.a != 0) {
        normalized_color[0] = text_color.r / 255.f;
        normalized_color[1] = text_color.g / 255.f;
        normalized_color[2] = text_color.b / 255.f;
        normalized_color[3] = text_color.a / 255.f;
    }
    SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &normalized_color, sizeof(normalized_color));

    SDL_GPUTextureSamplerBinding samplerBinding = SDL_GPUTextureSamplerBinding{ .texture = text_texture, .sampler = sampler };
    SDL_BindGPUFragmentSamplers(graphics::render_pass, 0, &samplerBinding, 1);
    SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);


    // SDL_BindGPUGraphicsPipeline(graphics::render_pass, pipeline);

    // float normalized_color[4]{text_color.r / 255.f, text_color.g / 255.f, text_color.b / 255.f, text_color.a / 255.f};
    // SDL_PushGPUVertexUniformData(graphics::cmdbuf, 0, &normalized_color, sizeof(normalized_color));

    // for (auto it = glyphs.begin(); it != glyphs.end(); it++) {
    //     SDL_GPUViewport scaled_viewport = it->second;
    //     scaled_viewport.x += position.x;
    //     scaled_viewport.y += position.y;
    //     scaled_viewport.x *= graphics::zoom[0] * text_scale;
    //     scaled_viewport.w *= graphics::zoom[0] * text_scale;
    //     scaled_viewport.y *= graphics::zoom[1] * text_scale;
    //     scaled_viewport.h *= graphics::zoom[1] * text_scale;

    //     SDL_SetGPUViewport(graphics::render_pass, &scaled_viewport);
    
    //     SDL_GPUTextureSamplerBinding samplerBinding = (SDL_GPUTextureSamplerBinding){ .texture = it->first, .sampler = sampler };
    //     SDL_BindGPUFragmentSamplers(graphics::render_pass, 0, &samplerBinding, 1);
    //     SDL_DrawGPUPrimitives(graphics::render_pass, 6, 1, 0, 0);
    // }

}

void Text::free() {
    // Logger::log_debug("Text freed");
}
