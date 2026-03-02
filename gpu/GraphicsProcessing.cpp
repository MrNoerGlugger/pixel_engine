#include "GraphicsProcessing.hpp"

map<string*, Drawable*> GraphicsProcessing::screen_map;
string GraphicsProcessing::texture_shader_map = "";
string GraphicsProcessing::texture_main_target = "";
string GraphicsProcessing::texture_overlay_target = "";
Texture GraphicsProcessing::texture_main = Texture();
Texture GraphicsProcessing::texture_overlay = Texture();
ShaderMap GraphicsProcessing::shader_map = ShaderMap();

void GraphicsProcessing::add_screen(Drawable* screen, string* screen_id) {
    screen_map.insert({screen_id, screen});
}
Drawable* GraphicsProcessing::get_screen(string* screen_id) {
    if (screen_map.count(screen_id) == 0) {
        return nullptr;
    }
    return screen_map.at(screen_id);
}

void GraphicsProcessing::interpret_window_scale() {
    int window_width = 1;
    int window_height = 1;
    SDL_GetWindowSize(graphics::window, &window_width, &window_height);
    graphics::zoom[0] = 1.f * window_width / SCREEN_WIDTH;
    graphics::zoom[1] = 1.f * window_height / SCREEN_HEIGHT;
    float general_zoom = graphics::zoom[0] > graphics::zoom[1] ? graphics::zoom[1] : graphics::zoom[0];
    graphics::window_scale[0] = graphics::zoom[0] / general_zoom;
    graphics::window_scale[1] = graphics::zoom[1] / general_zoom;
    graphics::zoom[0] = general_zoom;
    graphics::zoom[1] = general_zoom;
}

void GraphicsProcessing::resize_textures() {
    int current_screen_width = screen_map.at(graphics::current_screen)->get_bounds().w;
    int current_screen_height = screen_map.at(graphics::current_screen)->get_bounds().h;
    if (current_screen_width != graphics::get_texture(graphics::shadow_map_path, false).second.x || current_screen_height != graphics::get_texture(graphics::shadow_map_path, false).second.y) {
        graphics::register_target_texture(current_screen_width, current_screen_height, graphics::shadow_map_path );
        graphics::register_target_texture(current_screen_width, current_screen_height, texture_shader_map        );
        graphics::register_target_texture(current_screen_width, current_screen_height, texture_main_target       );
        shader_map.set_image_path(&texture_shader_map);
        texture_main.set_image_path(&texture_main_target);
    }
    int current_window_width = (int)(SCREEN_WIDTH * graphics::window_scale[0]);
    int current_window_height = (int)(SCREEN_HEIGHT * graphics::window_scale[1]);
    if (current_window_width != graphics::get_texture(texture_overlay_target, false).second.x || current_window_height != graphics::get_texture(texture_overlay_target, false).second.y) {
        graphics::register_target_texture(current_window_width, current_window_height, texture_overlay_target);
        texture_overlay.set_image_path(&texture_overlay_target);
    }
}

void GraphicsProcessing::init() {
    graphics::shadow_map_path   = graphics::register_target_texture(0, 0, graphics::shadow_map_path);
    texture_shader_map          = graphics::register_target_texture(0, 0, texture_shader_map       );
    texture_main_target         = graphics::register_target_texture(0, 0, texture_main_target      );
    texture_overlay_target      = graphics::register_target_texture(0, 0, texture_overlay_target   );

    //set the shader_map to use the texture_shader_map for the draw process
    shader_map.set_image_path(&texture_shader_map);
    shader_map.init();

    //set the texture_main to use the texture_main_target for the draw process
    texture_main.set_image_path(&texture_main_target);
    texture_main.init();

    //set the texture_overlay to use the texture_overlay_target for the draw process
    texture_overlay.set_image_path(&texture_overlay_target);
    texture_overlay.init();
}


void GraphicsProcessing::graphics_processing() {
    interpret_window_scale();
    resize_textures();

    //acquire the command buffer to start the draw process
    graphics::cmdbuf = SDL_AcquireGPUCommandBuffer(graphics::device);
    if (graphics::cmdbuf == NULL)
    {
        Logger::log_error(std::format("AcquireGPUCommandBuffer failed: {}", SDL_GetError()));
        return;
    }

    //update the geometry of the current screen
    update_geometry_all();

    //draw the shader map for the current screen
    draw_shader_map();


    // string current_texture_path = texture_shader_map;
    // int target_width = graphics::get_texture(current_texture_path, false).second.x;
    // int target_height = graphics::get_texture(current_texture_path, false).second.y;
    // const SDL_GPUTransferBufferCreateInfo buffer_create_info(
    //     SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD,
    //     target_width * target_height * 4,
    //     0
    // );
    // SDL_GPUTransferBuffer* buffer = SDL_CreateGPUTransferBuffer(graphics::device, &buffer_create_info);
    // if (target_width > 0 && target_height > 0) {
    //     const SDL_GPUTextureRegion source(
    //         graphics::get_texture(current_texture_path, false).first,
    //         0,
    //         0,
    //         0,
    //         0,
    //         0,
    //         target_width,
    //         target_height,
    //         1
    //     );
    //     const SDL_GPUTextureTransferInfo transfer_info(
    //         buffer,
    //         0,
    //         0,
    //         0
    //     );
    //     SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(graphics::cmdbuf);
    //     SDL_DownloadFromGPUTexture(copy_pass, &source, &transfer_info);
    //     SDL_EndGPUCopyPass(copy_pass);
    // }
    
    

    //draw the default phase for the current screen
    draw_default_phase();

    //draw the overlay of the current screen
    draw_overlay_phase();


    //acquire the swapchain texture for drawing onto the screen
    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(graphics::cmdbuf, graphics::window, &swapchainTexture, NULL, NULL)) {
        Logger::log_error(std::format("WaitAndAcquireGPUSwapchainTexture failed: {}", SDL_GetError()));
        return;
    }

    //draw the texture_main to the swapchain texture
    if (swapchainTexture != NULL) {
        SDL_GPUColorTargetInfo colorTargetInfo = {
            .texture = swapchainTexture,
            .clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f),
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE
        };

        graphics::render_pass = SDL_BeginGPURenderPass(graphics::cmdbuf, &colorTargetInfo, 1, NULL);

        interpret_window_scale();
        graphics::draw_stage = DEFAULT;

        texture_main.draw();
        texture_overlay.draw();

        graphics::end_render_pass();
    }

    //submit command buffer & wait for the gpu fence to make sure no memory leaks happen
	const auto submit_fence = SDL_SubmitGPUCommandBufferAndAcquireFence(graphics::cmdbuf);
    SDL_WaitForGPUFences(graphics::device, true, &submit_fence, 1);
    SDL_ReleaseGPUFence(graphics::device, submit_fence);

    // if (target_width > 0 && target_height > 0) {
    //     SDL_Surface* shader_map_surface = SDL_CreateSurfaceFrom(
    //           target_width
    //         , target_height
    //         , SDL_PIXELFORMAT_RGBA32
    //         , SDL_MapGPUTransferBuffer(graphics::device, buffer, false)
    //         , target_width * 4);
    //     path file_path = FilePath::filePath;
    //     file_path += "shader_map_surface.png";
    //     if (IMG_Save(shader_map_surface, file_path.c_str())) {
    //         Logger::log_info("Saved shader map to file!");
    //     }
    //     else {
    //         Logger::log_error(std::format("Failed to save shader map to file: {}", SDL_GetError()));
    //     }

    //     SDL_UnmapGPUTransferBuffer(graphics::device, buffer);
    // }
    // SDL_ReleaseGPUTransferBuffer(graphics::device, buffer);
}

void GraphicsProcessing::update_geometry_all() {
    screen_map.at(graphics::current_screen)->update_geometry();
}

void GraphicsProcessing::draw_shader_map() {
    draw_shadow_phase();
    draw_light_phase();
}

void GraphicsProcessing::draw_shadow_phase() {
    if (graphics::get_texture(graphics::shadow_map_path, false).first == NULL) {
        Logger::log_error("failed to create shadow_map_path!");
        return;
    }
    SDL_GPUColorTargetInfo colorTargetInfo = {  
        .texture = graphics::get_texture(graphics::shadow_map_path, false).first,
        .clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 0.0f),
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    graphics::render_pass = SDL_BeginGPURenderPass(graphics::cmdbuf, &colorTargetInfo, 1, NULL);

    graphics::draw_stage = SHADOW;
    
    graphics::zoom[0] = 1.f;
    graphics::zoom[1] = 1.f;

    screen_map.at(graphics::current_screen)->draw();

    graphics::end_render_pass();
}

void GraphicsProcessing::draw_light_phase() {
    if (graphics::get_texture(texture_shader_map, false).first == NULL) {
        Logger::log_error("failed to create texture_shader_map!");
        return;
    }
    SDL_GPUColorTargetInfo colorTargetInfo = {  
        .texture = graphics::get_texture(texture_shader_map, false).first,
        .clear_color = SDL_FColor(0.25f, 0.25f, 0.25f, 1.f),
        // .clear_color = SDL_FColor(0.5f, 0.5f, 0.5f, 1.f),
        // .clear_color = SDL_FColor(0.f, 0.f, 0.f, 0.f),
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    graphics::render_pass = SDL_BeginGPURenderPass(graphics::cmdbuf, &colorTargetInfo, 1, NULL);
    
    graphics::draw_stage = LIGHT;

    graphics::zoom[0] = 1.f;
    graphics::zoom[1] = 1.f;

    screen_map.at(graphics::current_screen)->draw();

    graphics::end_render_pass();

}

void GraphicsProcessing::draw_default_phase() {
    if (graphics::get_texture(texture_main_target, false).first == NULL) {
        Logger::log_error("failed to create texture_main_target!");
        return;
    }
    SDL_GPUColorTargetInfo colorTargetInfo = {
        .texture = graphics::get_texture(texture_main_target, false).first,
        .clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f),
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    graphics::render_pass = SDL_BeginGPURenderPass(graphics::cmdbuf, &colorTargetInfo, 1, NULL);


    graphics::draw_stage = DEFAULT;

    graphics::zoom[0] = 1.f;
    graphics::zoom[1] = 1.f;

    screen_map.at(graphics::current_screen)->draw();
    shader_map.draw();

    graphics::end_render_pass();
}

void GraphicsProcessing::draw_overlay_phase() {
    if (graphics::get_texture(texture_overlay_target, false).first == NULL) {
        Logger::log_error("failed to create texture_overlay_target!");
        return;
    }
    SDL_GPUColorTargetInfo colorTargetInfo = {
        .texture = graphics::get_texture(texture_overlay_target, false).first,
        .clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f),
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };

    graphics::render_pass = SDL_BeginGPURenderPass(graphics::cmdbuf, &colorTargetInfo, 1, NULL);


    graphics::draw_stage = OVERLAY;

    graphics::zoom[0] = 1.f;
    graphics::zoom[1] = 1.f;

    screen_map.at(graphics::current_screen)->draw();

    graphics::end_render_pass();
}


void GraphicsProcessing::set_view(float x, float y) {
    texture_main.set_position(x, y);
    graphics::view_position[0] = x;
    graphics::view_position[1] = y;
}

void GraphicsProcessing::move_view(float x, float y) {
    texture_main.move(x, y);
    graphics::view_position[0] += x;
    graphics::view_position[1] += y;
}