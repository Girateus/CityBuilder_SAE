# Cmake Version
cmake_minimum_required(VERSION 3.30)

# Create Solution
project(CityBuilder)

# C++ Version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Creating Architecture and libraries
add_subdirectory(core/)
add_subdirectory(api/)
add_subdirectory(game/)

# Manage Ressources
# Delete all and copy
#file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/_assets")
#file(COPY ${CMAKE_SOURCE_DIR}/_assets DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/_assets ${CMAKE_BINARY_DIR}/_assets
        COMMENT "🔄 Copying all resources to build directory"
)

# Add executable project
add_executable(city_builder_game main.cc)
add_dependencies(city_builder_game copy_resources)

# This project is linked to its own libraries
target_link_libraries(city_builder_game PRIVATE game api core)
