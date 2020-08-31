file(GLOB SRC "*.cpp")
add_library(imgui STATIC ${SRC})
target_link_libraries(imgui PRIVATE sdl2::sdl2-noprefix glew::glew freetype::freetype common)
target_compile_definitions(imgui INTERFACE IMGUI_IMPL_OPENGL_LOADER_GLEW)
