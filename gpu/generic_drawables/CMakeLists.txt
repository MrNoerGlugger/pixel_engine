add_library(generic_drawables STATIC
    Rectangle.cpp
    Rectangle.hpp
    BasicRhombus.cpp
    BasicRhombus.hpp
    Drawable.cpp
    Drawable.hpp
    LightSource.cpp
    LightSource.hpp
    ShaderMap.cpp
    ShaderMap.hpp
    ShaderTexture.cpp
    ShaderTexture.hpp
    Text.cpp
    Text.hpp
    Texture.cpp
    Texture.hpp
    TextureSheet.cpp
    TextureSheet.hpp
)

target_include_directories(generic_drawables PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(generic_drawables PUBLIC shader_handling textures functional_base)