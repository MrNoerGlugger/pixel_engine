#pragma once

#include <string>
#include <GPUAccessables.hpp>
#include <Logger.hpp>

namespace graphics {
	typedef enum resource_type {
		TEXTURES,
		MUSIC,
		SOUNDS,
		SHADERS
	} ResourceType;

	typedef enum symbol_type {
		DAMAGE_TYPE,
		ITEM_TYPE
	} SymbolType;

	extern std::string get_damage_symbol(int id);
	extern std::string get_item_symbol(int id);

	extern void add_resource_path(std::string* resource_id, std::string resource_path);
	extern std::string get_resource_path(ResourceType resource_type, std::string* resource_id, std::string file_name);
};

