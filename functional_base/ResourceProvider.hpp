#pragma once

#include <string>

namespace graphics {
	typedef enum resource_path {
		ATTACKS,
		BACKGROUND,
		CHARACTERS,
		DUNGEON,
		FONT,
		ITEMS,
		OVERLAY,
		ROOM,
		SHADERS,
		TEXTBOX
	} ResourcePath;

	typedef enum symbol_type {
		DAMAGE_TYPE,
		ITEM_TYPE
	} SymbolType;

	extern std::string get_damage_symbol(int id);
	extern std::string get_item_symbol(int id);

	extern std::string get_resource_path(ResourcePath resource_path, std::string file_name);
};

