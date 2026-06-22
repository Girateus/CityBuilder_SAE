find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cc)
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(api ${SRC_FILES} ${HEADER_FILES}
        src/ai/npc_manager.cc
        include/ai/npc_manager.h
        include/ai/a_star_graph.h
        src/ai/a_star_graph.cc)

target_include_directories(api PUBLIC include/)
target_link_libraries(api PUBLIC SFML::System SFML::Graphics SFML::Window)
target_link_libraries(api PUBLIC core)
