#include "CustomText.hpp"
#include "../../../init/Textures.hpp"

CustomText::CustomText() {}

CustomText::CustomText(string text_string) {
    this->text_string = text_string;
    geometry_update_needed = true;
}

void CustomText::set_text_string(string text_string) {
    this->text_string = text_string;
    geometry_update_needed = true;
}
void CustomText::set_single_glyphs(bool single_glyphs) {
    this->single_glyphs = single_glyphs;
    geometry_update_needed = true;
}


void createGlyph(string& text_string, vector<Drawable*>& glyphs, SDL_Color& text_color) {
    glyphs.push_back(new Text(text_string));

    Text* text = (Text*)glyphs.back();

    if (text_color.r != 0 || text_color.g != 0 || text_color.b != 0 || text_color.a != 0) {
        text->set_text_color(text_color);
    }
}

void createSymbol(graphics::SymbolType type, int id, vector<Drawable*>& glyphs) {
    glyphs.push_back(new TextureSheet());

    TextureSheet* sheet = (TextureSheet*)glyphs.back();
    switch (type) {
    case graphics::DAMAGE_TYPE:
        sheet->set_image_path(&textures::element_sheet_small);
        sheet->set_part(id);
        break;

    case graphics::ITEM_TYPE:
        if (id < 3) {
            sheet->set_image_path(&textures::item_sheets[textures::COIN]);
            sheet->set_part(id);
        }
        else if (id < 5) {
            sheet->set_image_path(&textures::item_sheets[textures::KEY]);
            sheet->set_part(id - 3);
        }
        else {
            sheet->set_image_path(&textures::item_sheets[textures::NUT]);
            sheet->set_part(id - 5);
        }
        break;
    }
}

void CustomText::update_geometry() {
    if (!geometry_update_needed) {
        return;
    }
    geometry_update_needed = false;
    free();
    glyphs.clear();

    string glyph_string = "";
    bool string_end = false;
    int glyph_size = 0;
    bool symbol = false;
    graphics::SymbolType type = graphics::DAMAGE_TYPE;

    float xPos = position.x;
    float yPos = position.y;
    float maxX = position.x;
    float maxY = position.y;
    for (auto it = text_string.begin(); it != text_string.end(); it++) {
        if (symbol) {
            symbol = false;
            int id = *it - 3;
            createSymbol(type, id, glyphs);
        }
        else if (*it == char(1)) {
            string_end = true;
            type = graphics::DAMAGE_TYPE;
            symbol = true;
        }
        else if (*it == char(2)) {
            string_end = true;
            type = graphics::ITEM_TYPE;
            symbol = true;
        }
        else if (*it == '\n') {
            string_end = true;
            if (glyphs.size() != 0) {
                xPos = position.x;
                yPos = maxY;
            }
        }
        else {
            glyph_string += *it;
            if (single_glyphs) {
                string_end = true;
            }
        }

        if (string_end && glyph_string != "") {
            createGlyph(glyph_string, glyphs, text_color);

            string_end = false;
            glyph_string = "";
        }

        if (glyph_size != glyphs.size()) {
            glyph_size = glyphs.size();

            glyphs.back()->set_position(xPos, yPos);
            glyphs.back()->set_scale(scale.x, scale.y);
            glyphs.back()->init();
            SDL_FRect bounds = glyphs.back()->get_bounds();

            Logger::log_debug(std::format("CustomText Bounds: {}:x,y:{}\t{}:w,h:{}", bounds.x, bounds.y, bounds.w, bounds.h));

            xPos += bounds.w;
            maxX = maxX < xPos ? xPos : maxX;
            maxY = maxY < bounds.y + bounds.h ? bounds.y + bounds.h : maxY; 
        }
    }

    if (glyph_string != "") {
        createGlyph(glyph_string, glyphs, text_color);
        glyph_size = glyphs.size();

        glyphs.back()->set_position(xPos, yPos);
        glyphs.back()->set_scale(scale.x, scale.y);
        glyphs.back()->init();
        SDL_FRect bounds = glyphs.back()->get_bounds();

        xPos = maxX;
        maxX += bounds.w;
        maxY = maxY < bounds.y + bounds.h ? bounds.y + bounds.h : maxY; 
    }

    viewport =  {(float)position.x, (float)position.y, (float)(maxX - position.x), (float)(maxY - position.y), 0.0f, 1.0f};

    Logger::log_debug(std::format("CustomText Glyph Amount:{}", glyphs.size()));
}

void CustomText::init() {
    update_geometry();
}

void CustomText::draw() {
    // update_geometry();
    for (auto it = glyphs.begin(); it != glyphs.end(); it++) {
        (*it)->draw();
    }
}

void CustomText::free() {
    for (auto it = glyphs.begin(); it != glyphs.end(); it++) {
        delete (*it);
    }
    // Logger::log_debug("CustomText freed");
}