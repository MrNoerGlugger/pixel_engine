add_library(gui_base STATIC
    ButtonBase.cpp
    ButtonBase.hpp
    TextboxBase.cpp
    TextboxBase.hpp
)#

target_include_directories(gui_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(gui_base PUBLIC functional_base specialized_drawables compiler_flags)