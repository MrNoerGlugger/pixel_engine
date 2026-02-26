add_library(specialized_drawables STATIC
    CustomText.cpp
    CustomText.hpp
    Screen.cpp
    Screen.hpp
)

target_include_directories(specialized_drawables PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(specialized_drawables PUBLIC generic_drawables functional_base)