add_subdirectory(generic_drawables)
add_subdirectory(specialized_drawables)
add_library(shader_handling STATIC
    ShaderHandling.cpp
    ShaderHandling.hpp
    TextureInit.cpp
    TextureInit.hpp
)
add_library(gpu STATIC
    GraphicsProcessing.cpp
    GraphicsProcessing.hpp
)

target_include_directories(shader_handling PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(gpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(shader_handling PUBLIC functional_base sdl_all)
target_link_libraries(gpu PUBLIC specialized_drawables generic_drawables shader_handling functional_base sdl_all)