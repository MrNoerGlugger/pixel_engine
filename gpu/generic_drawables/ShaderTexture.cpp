#include "ShaderTexture.hpp"

void ShaderTexture::set_image_path(std::string* image_path, bool shader_texture) {
    texture_default.set_image_path(image_path, shader_texture);
}
void ShaderTexture::set_image_path_shader(std::string* image_path, bool shader_texture) {
    TextureSheet::set_image_path(image_path, shader_texture);
}
void ShaderTexture::set_light_sources_active(bool light_sources_active) {
    this->light_sources_active = light_sources_active;
}


void ShaderTexture::set_part(int part) {
    TextureSheet::set_part(part);
    texture_default.set_part(part);
}
void ShaderTexture::set_mirroring(float mirroring) {
    TextureSheet::set_mirroring(mirroring);
    texture_default.set_mirroring(mirroring);
    for (auto source : sources) {
        source->set_mirroring(mirroring);
    }
}
void ShaderTexture::set_rotation(float rotation) {
    TextureSheet::set_rotation(rotation);
    texture_default.set_rotation(rotation);
    for (auto source : sources) {
        source->set_rotation(rotation);
    }
}


void ShaderTexture::set_position(float x, float y) {
    TextureSheet::set_position(x, y);
    texture_default.set_position(x, y);
    for (auto source : sources) {
        source->set_position(x, y);
    }
}
void ShaderTexture::move(float x, float y) {
    TextureSheet::move(x, y);
    texture_default.move(x, y);
    for (auto source : sources) {
        source->move(x, y);
    }
}

void ShaderTexture::init() {
    free();
    sources.clear();

    TextureSheet::init();
    texture_default.init();

    ShaderData* shader_data = graphics::get_shader_data(*image_path);
    if (shader_data == NULL) {
        Logger::log_error(std::format("Failed to load shader_data at {}{}{}", '"', *image_path, '"'));
        return;
    }
    for (auto source : shader_data->lightsources) {
        SDL_FPoint light_source_position = {(float)source.origin.x, (float)source.origin.y};
        if (!SDL_PointInRectFloat(&light_source_position, &parts->at(part))) {
            continue;
        }
        sources.push_back(new LightSource());
        LightSource* new_source = (LightSource*)sources.back();
        new_source->set_texture_size({(int)parts->at(part).w, (int)parts->at(part).h});
        new_source->set_color(source.lightsource_color);
        new_source->set_origin({(int)(source.origin.x - parts->at(part).x), (int)(source.origin.y - parts->at(part).y)});
        new_source->set_position(position.x, position.y);
        new_source->set_mirroring(mirroring);
        new_source->set_rotation(rotation);
        new_source->set_image_path(&graphics::shadow_map_path);
        new_source->init();
    }
}

void ShaderTexture::draw() {
    switch (graphics::draw_stage) {
    case DEFAULT:
        texture_default.draw();
        break;
    case SHADOW:
        TextureSheet::draw();
        break;
    case LIGHT:
        if (!light_sources_active) {
            break;
        }
        for (auto source : sources) {
            source->draw();
        }
        break;
    case OVERLAY:
        break;
    }
}

void ShaderTexture::free() {
    for (auto it = sources.begin(); it != sources.end(); it++) {
        delete (*it);
    }
    // Logger::log_debug("ShaderTexture freed");
}