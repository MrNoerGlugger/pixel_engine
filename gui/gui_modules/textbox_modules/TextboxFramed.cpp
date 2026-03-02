#include "TextboxFramed.hpp"
#include "TextureSheet.hpp"

////////////////////////////////////////////////////////////
TextboxFramed::TextboxFramed() :
corners						(nullptr),
edges						(nullptr),
texture_size                ({0, 0}),
size                        (),
background                  (),
frame_parts                 (),
TextboxFramed::TextboxBase  ()
{
    
}


////////////////////////////////////////////////////////////
TextboxFramed::TextboxFramed(vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable) :
corners						(nullptr),
edges						(nullptr),
texture_size                ({0, 0}),
size                        (),
background                  (),
frame_parts                 (),
TextboxFramed::TextboxBase  (buttons, pos_visual, textbox_id, interactable)
{
    
}


////////////////////////////////////////////////////////////
TextboxFramed::TextboxFramed(const SDL_Point& size, vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable) :
corners						(nullptr),
edges						(nullptr),
texture_size                ({0, 0}),
size                        (size),
background                  (),
frame_parts                 (),
TextboxFramed::TextboxBase  (buttons, pos_visual, textbox_id, interactable)
{
    
}

void TextboxFramed::set_image_paths(std::string* corners, std::string* edges) {
	this->corners = corners;
	this->edges = edges;
	TextureSheet temp_sheet;
	temp_sheet.set_image_path(corners);
	temp_sheet.init();
	auto parts = graphics::texture_parts_map.at(*corners);
	texture_size = {(int)parts.begin()->w, (int)parts.begin()->h};
	
	geometry_update_needed = true;
}


void TextboxFramed::createTextboxFrame(const SDL_Point& size) const 
{
	frame_parts.clear();
    SDL_FPoint base_pos{position.x, position.y};
	SDL_FPoint pos{0.f, 0.f};
	float rotation = 0.f;
	for (int x = 0; x < size.x; x++) {
		pos = {x * texture_size.x * 1.f, 0.f};
		rotation = 0.f;
		if (x == 0 || x == size.x - 1) {
			if (x == size.x - 1) {
				rotation = 90.f;
			}
			TextureSheet corner;
			corner.set_image_path(corners);
			corner.set_part(graphics::frameType);
			corner.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
			corner.set_rotation(rotation);
			frame_parts.push_back(corner);
		}
		else {
			TextureSheet edge;
			edge.set_image_path(edges);
			edge.set_part(graphics::frameType);
			edge.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
			edge.set_rotation(rotation);
			frame_parts.push_back(edge);
		}
		pos = {x * texture_size.x * 1.f, (size.y - 1) * texture_size.y * 1.f};
		rotation = 180.f;
		if (x == 0 || x == size.x - 1) {
			if (x == 0) {
				rotation = 270.f;
			}
			TextureSheet corner;
			corner.set_image_path(corners);
			corner.set_part(graphics::frameType);
			corner.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
			corner.set_rotation(rotation);
			frame_parts.push_back(corner);
		}
		else {
			TextureSheet edge;
			edge.set_image_path(edges);
			edge.set_part(graphics::frameType);
			edge.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
			edge.set_rotation(rotation);
			frame_parts.push_back(edge);
		}
	}
	for (int y = 1; y < size.y - 1; y++) {
		pos = {0.f, y * texture_size.y * 1.f};
		rotation = 270.f;
		TextureSheet edge;
		edge.set_image_path(edges);
		edge.set_part(graphics::frameType);
		edge.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
		edge.set_rotation(rotation);
		frame_parts.push_back(edge);
		pos = {(size.x - 1) * texture_size.x * 1.f, y * texture_size.y * 1.f};
		rotation = 90.f;
		TextureSheet edge2;
		edge2.set_image_path(edges);
		edge2.set_part(graphics::frameType);
		edge2.set_position(base_pos.x + pos.x, base_pos.y + pos.y);
		edge2.set_rotation(rotation);
		frame_parts.push_back(edge2);
	}
	for (auto it = frame_parts.begin(); it != frame_parts.end(); it++) {
		it->init();
	}
}


////////////////////////////////////////////////////////////
void TextboxFramed::draw()
{
	if (!displayed) return;

    update_geometry();
    
	for (auto it = frame_parts.begin(); it != frame_parts.end(); it++) {
		it->draw();
	}
	background.draw();

    TextboxBase::draw();
}


void TextboxFramed::update_geometry()
{
    // Do nothing, if geometry has not changed
    if (!geometry_update_needed)
        return;

    position.x += texture_size.x;
    position.y += texture_size.y;

    // update TextboxBase geometry first
    TextboxBase::update_geometry();
    
    SDL_Point size_t;
    if (size.x != 0 || size.y != 0) {
        size_t = size;
    }
    else {
        size_t.x = ceil(1.0 * bounds.w / texture_size.x) + 2;
        size_t.y = ceil(1.0 * bounds.h / texture_size.y) + 2;
    }

	background.set_size((size_t.x - 2) * texture_size.x * 1.f, (size_t.y - 2) * texture_size.y * 1.f);
	background.set_position(position.x, position.y);
    background.set_color(*graphics::background);

    position.x -= texture_size.x;
    position.y -= texture_size.y;
    createTextboxFrame(size_t);     

	//update bounds to include the frame
    bounds.x = position.x;
    bounds.y = position.y;
    bounds.w  = size_t.x * texture_size.x * 1.f;
    bounds.h = size_t.y * texture_size.y * 1.f;

    createBoundingBox();
}