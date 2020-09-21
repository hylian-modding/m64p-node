file(GLOB SRC "*.cpp")
add_library(sdl STATIC ${SRC})
target_compile_features(sdl PUBLIC cxx_std_17)
target_link_libraries(sdl
    INTERFACE common sdl2::sdl2
    PRIVATE common sdl2::sdl2 sdl2::sdl2-ttf sdl2::sdl2-image fmt::fmt)
