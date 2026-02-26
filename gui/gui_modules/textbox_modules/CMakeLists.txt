add_library(textbox_modules STATIC
    TextboxFramed.cpp
    TextboxFramed.hpp
)#

target_include_directories(textbox_modules INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(textbox_modules PUBLIC gui_base compiler_flags)