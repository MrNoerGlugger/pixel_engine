add_library(button_modules STATIC
    ButtonText.cpp
    ButtonText.hpp
)#

target_include_directories(button_modules INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(button_modules PUBLIC gui_base compiler_flags)