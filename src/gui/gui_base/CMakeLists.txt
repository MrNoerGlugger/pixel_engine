add_library(gui_base STATIC
    TextboxBase.cpp
    TextboxBase.hpp
)#
add_library(button_base STATIC
    ButtonBase.cpp
    ButtonBase.hpp
)

target_include_directories(button_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(gui_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(button_base PUBLIC input generic_drawables functional_base compiler_flags)
target_link_libraries(gui_base PUBLIC button_base compiler_flags)