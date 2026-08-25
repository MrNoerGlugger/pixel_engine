add_library(input STATIC
    GamepadHandling.cpp
    GamepadHandling.hpp
    InputBase.cpp
    InputBase.hpp
    InputHandling.cpp
    InputHandling.hpp
)#

target_include_directories(input INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries(input PUBLIC sdl_all compiler_flags)