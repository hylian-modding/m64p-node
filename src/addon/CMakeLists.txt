file(GLOB SRC "*.cpp" "**/**.cpp")
add_library(addon SHARED ${SRC} ${CMAKE_JS_SRC})
target_compile_features(addon PRIVATE cxx_std_17)

set_target_properties(addon PROPERTIES
    PREFIX ""
    SUFFIX ".node"
    OUTPUT_NAME "m64p")

if(UNIX)
    set_target_properties(addon PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR)/bin")
endif()

target_link_libraries(addon
    PUBLIC node::cmake-js node::node-addon-api
    PRIVATE common frontend imgui m64p fmt::fmt)

if(WIN32)
	target_compile_definitions(addon PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
