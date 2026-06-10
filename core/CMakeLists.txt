##file(GLOB_RECURSE SRC_FILES src/*.cc)
##file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(core INTERFACE
        include/ai/bt_action.h
        include/ai/bt_sequence.h
        include/ai/bt_selector.h
        include/ai/bt_composite.h
        include/ai/bt_node_factory.h)

target_include_directories(core INTERFACE include/)
