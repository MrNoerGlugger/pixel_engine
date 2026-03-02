add_library(specialized_drawables STATIC
    Screen.cpp
    Screen.hpp
)

target_include_directories(specialized_drawables PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(specialized_drawables PUBLIC gui_base functional_base)