#include "TextureInit.hpp"

void graphics::register_texture(string path) {

    if (graphics::texture_map.count(path) != 0) {
        return;
    }

    SDL_Surface* imageData = IMG_Load(path.c_str());
    imageData->format = SDL_PIXELFORMAT_RGBA32;
    if (imageData == NULL) 
    {
        SDL_Log("Could not load image data!");
        return;
    }
    
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


    graphics::texture_map.insert({path, {texture, textureSize}});
}

void graphics::register_shader_texture(string path) {

    if (graphics::shader_texture_map.count(path) != 0) {
        return;
    }

    SDL_Surface* imageData = IMG_Load(path.c_str());
    imageData->format = SDL_PIXELFORMAT_RGBA32;
    if (imageData == NULL) 
    {
        Logger::log_error("Could not load image data!");
        return;
    }
    
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

    ShaderData shader;

    shader.shader_texture = SDL_CreateGPUTexture(graphics::device, &textureCreateInfo);

    shader.shader_size = SDL_Point{imageData->w, imageData->h};

    SDL_GPUTransferBufferCreateInfo bufferCreateInfo = SDL_GPUTransferBufferCreateInfo {
        SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        (Uint32)(imageData->w * imageData->h * 4),
        0
    };

    SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(graphics::device, &bufferCreateInfo);

    Uint8* textureTransferPointer = (Uint8*)SDL_MapGPUTransferBuffer(graphics::device, textureTransferBuffer, false);

    if (imageData->pixels == NULL) {
        Logger::log_error("Failed to open the pixel-data!");
        return;
    }

    vector<pair<Uint32, int>> lightsources;
    Uint32* pixels = (Uint32*) imageData->pixels;
    for (int i = 0; i < imageData->w * imageData->h; i++) {
        //we check for any color so we know that a lightsource is there; with modulo 0x01000000 we get the first 24 bits of the Uint32 which are r(0-7)g(8-15)b(16-23)
        if (pixels[i] % 0x01000000 != 0) {
            lightsources.push_back({pixels[i], i});
            pixels[i] = 0;
            continue;
        }
        //this is just for testing purposes and generally working with hex numbers
        // if (pixels[i] != 0) {
        //     pixels[i] = 0x1E1E1Eu + pixels[i];
        //     continue;
        // }
    }

    // creating lightsource to use for shaders
    for (auto lightsource : lightsources) {

        //light source size is now calculated at the creation of the lightsource
        // //lightsource size is just the alpha of the lightsource * 2 (because it's a radius) - 1 (because the center is double-counted)
        // int size = lightsource.first / 0x01000000 * 2 - 1;
        // SDL_Point lightsource_size = { size, size };

        SDL_Point lightsource_origin = { lightsource.second % imageData->w, lightsource.second / imageData->w };
        //lightsource color is equal to the Uint32 saved above
        shader.lightsources.push_back({lightsource_origin, lightsource.first});
    }


    SDL_memcpy(textureTransferPointer, pixels, imageData->w * imageData->h * 4);
    SDL_UnmapGPUTransferBuffer(graphics::device, textureTransferBuffer);


    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(graphics::device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_GPUTextureTransferInfo textureTransferInfo = SDL_GPUTextureTransferInfo {
        .transfer_buffer = textureTransferBuffer,
        .offset = 0
    };
    SDL_GPUTextureRegion textureRegion = SDL_GPUTextureRegion {
        .texture = shader.shader_texture,
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


    graphics::shader_texture_map.insert({path, shader});
}

string graphics::register_target_texture(int width, int height, string target) {
    if (target == "") {
        target = "texture_target_";
        target += std::to_string(graphics::gpu_target_texture_amount);
        graphics::gpu_target_texture_amount++; 
    }

    if (graphics::texture_map.count(target) != 0) {
        // Logger::log_debug(std::format("Resized target( {} ) to {}:{}", target, width, height));
        SDL_ReleaseGPUTexture(graphics::device, graphics::texture_map.at(target).first);
        graphics::texture_map.erase(target);
    }

    if (width < 0) {
        width = 0;
        Logger::log_warning(std::format("Width negative at {}", target));
    }
    if (height < 0) {
        height = 0;
        Logger::log_warning(std::format("Height negative at {}", target));
    }

    SDL_GPUTextureCreateInfo textureCreateInfo = SDL_GPUTextureCreateInfo {
        SDL_GPU_TEXTURETYPE_2D,
        SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
        (Uint32)width,
        (Uint32)height,
        1,
        1,
        SDL_GPU_SAMPLECOUNT_1,
        0
    };

    SDL_GPUTexture* texture = SDL_CreateGPUTexture(graphics::device, &textureCreateInfo);

    if (texture == NULL) {
        Logger::log_error(std::format("Failed to create GPUTexture( {} ) with {}:{}\tError: {}", target, width, height, SDL_GetError()));
    }

    SDL_Point textureSize = SDL_Point{width, height};

    graphics::texture_map.insert({target, {texture, textureSize}});

    return target;
}

pair<SDL_GPUTexture*, SDL_Point> graphics::get_texture(string path, bool shader) {
    if (!shader) {
        graphics::register_texture(path);
        return graphics::texture_map.at(path);
    }
    else {
        graphics::register_shader_texture(path);
        return {graphics::shader_texture_map.at(path).shader_texture, graphics::shader_texture_map.at(path).shader_size};
    }
}

ShaderData* graphics::get_shader_data(string path) {
    graphics::register_shader_texture(path);
    return &graphics::shader_texture_map.at(path);
}