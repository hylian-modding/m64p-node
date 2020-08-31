file(GLOB SRC "*.cpp")
add_library(stb STATIC ${SRC})
target_link_libraries(stb PRIVATE stb::stb INTERFACE stb::stb)
