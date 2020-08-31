file(GLOB SRC "*.cpp")
add_library(m64p STATIC ${SRC})
target_compile_features(m64p PUBLIC cxx_std_17)
target_link_libraries(m64p
    INTERFACE common m64p::m64p-api
    PRIVATE common sdl fmt::fmt m64p::m64p-api)

if(WIN32)
	target_compile_definitions(m64p PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
