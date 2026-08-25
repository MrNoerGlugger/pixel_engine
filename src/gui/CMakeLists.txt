add_subdirectory(gui_base)
add_subdirectory(gui_modules)
add_library(gui INTERFACE
)#

target_include_directories(gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(gui INTERFACE gui_base gui_modules compiler_flags)