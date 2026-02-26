#include "ResourceProvider.hpp"

std::string graphics::get_damage_symbol(int id) {
	std::string damage;
	damage += char(1);
	damage += char(3 + id);
	return damage;
}
std::string graphics::get_item_symbol(int id) {
	std::string item;
	item += char(2);
	item += char(3 + id);
	return item;
}

std::string graphics::get_resource_path(graphics::ResourcePath resource_path, std::string file_name) {
	std::string path = "resources/textures/";

	switch (resource_path)
	{
	case ATTACKS:
		path += "attacks/";
		break;

	case BACKGROUND:
		path += "background/";
		break;

	case CHARACTERS:
		path += "characters/";
		break;
	
	case DUNGEON:
		path += "dungeon/";
		break;
	
	case FONT:
		path += "font/";
		break;

	case ITEMS:
		path += "items/";
		break;

	case OVERLAY:
		path += "overlay/";
		break;

	case ROOM:
		path += "room/";
		break;

	case SHADERS:
		path += "shaders/";
		break;

	case TEXTBOX:
		path += "textbox/";
		break;
	}

	path += file_name;

	return path;
}