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

void graphics::add_resource_path(std::string* resource_id, std::string resource_path) {
	graphics::resource_path_map.insert({resource_id, resource_path});
}

std::string graphics::get_resource_path(ResourceType resource_type, std::string* resource_id, std::string file_name) {
	std::string path = "resources/";

	switch (resource_type)
	{
	case TEXTURES:
		path += "textures/";
		break;

	case MUSIC:
		path += "audio/music/";
		break;

	case SOUNDS:
		path += "audio/sounds/";
		break;
	
	case SHADERS:
		path += "shaders/";
		break;
	}

	if (graphics::resource_path_map.count(resource_id) == 0) {
		Logger::log_warning(std::format("resource_id: {} not found!", *resource_id));
	}
	else {
		path += graphics::resource_path_map.at(resource_id);
	}

	path += file_name;

	return path;
}