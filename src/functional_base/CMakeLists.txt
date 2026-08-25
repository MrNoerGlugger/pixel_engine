# add_library(filepath STATIC
#     FilePath.cpp
#     FilePath.hpp
# )

add_library(functional_base STATIC
    FilePath.cpp
    FilePath.hpp
    FunctionHolder.cpp
    FunctionHolder.hpp
    GenericAccessables.cpp
    GenericAccessables.hpp
    GPUAccessables.cpp
    GPUAccessables.hpp
    Logger.cpp
    Logger.hpp
    MapBase.hpp
    Position.cpp
    Position.hpp
    ResourceProvider.cpp
    ResourceProvider.hpp
)#

# target_include_directories(filepath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(functional_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# target_link_libraries(filepath PUBLIC compiler_flags)
target_link_libraries(functional_base PUBLIC sdl_all compiler_flags)