file(GLOB SRC "*.cpp")
add_library(common STATIC ${SRC})
target_compile_features(common PUBLIC cxx_std_17)
target_link_libraries(common PRIVATE fmt::fmt sdl2::sdl2)

if(WIN32)
	target_compile_definitions(common PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()

if(UNIX)
    find_package(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

    target_include_directories(common PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(common PRIVATE ${GTK3_LIBRARIES})
endif()
