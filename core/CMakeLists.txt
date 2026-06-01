##file(GLOB_RECURSE SRC_FILES src/*.cc)
##file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(core INTERFACE)

target_include_directories(core INTERFACE include/)
