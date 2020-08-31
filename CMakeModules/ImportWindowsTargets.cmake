if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BUILD_SUFFIX "d")
	set(BUILD_PATH "debug")
else()
	set(BUILD_SUFFIX "")
	set(BUILD_PATH "")
endif()

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(INCLUDE_DIR "$ENV{vcpkg_x64}/include")
    set(LIB_DIR "$ENV{vcpkg_x64}/${BUILD_PATH}/lib")
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(INCLUDE_DIR "$ENV{vcpkg_x86}/include")
    set(LIB_DIR "$ENV{vcpkg_x86}/${BUILD_PATH}/lib")
endif()

set(SDL2_LIBRARY "${LIB_DIR}/SDL2${BUILD_SUFFIX}.lib")

add_library(sdl2::sdl2 UNKNOWN IMPORTED)
set_target_properties(sdl2::sdl2 PROPERTIES IMPORTED_LOCATION "${SDL2_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")

add_library(sdl2::sdl2-noprefix UNKNOWN IMPORTED)
set_target_properties(sdl2::sdl2-noprefix PROPERTIES IMPORTED_LOCATION "${SDL2_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}/SDL2")

set(SDL2_TTF_LIBRARY "${LIB_DIR}/SDL2_ttf.lib")

add_library(sdl2::sdl2-ttf UNKNOWN IMPORTED)
set_target_properties(sdl2::sdl2-ttf PROPERTIES IMPORTED_LOCATION "${SDL2_TTF_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")

set(SDL2_IMAGE_LIBRARY "${LIB_DIR}/SDL2_image.lib")

add_library(sdl2::sdl2-image UNKNOWN IMPORTED)
set_target_properties(sdl2::sdl2-image PROPERTIES IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")

set(SDL2_MIXER_LIBRARY "${LIB_DIR}/SDL2_mixer.lib")

add_library(sdl2::sdl2-mixer UNKNOWN IMPORTED)
set_target_properties(sdl2::sdl2-mixer PROPERTIES IMPORTED_LOCATION "${SDL2_MIXER_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")

set(FREETYPE_LIBRARY "${LIB_DIR}/freetype${BUILD_SUFFIX}.lib")

add_library(freetype::freetype UNKNOWN IMPORTED)
set_target_properties(freetype::freetype PROPERTIES IMPORTED_LOCATION "${FREETYPE_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")

set(GLEW_LIBRARY "${LIB_DIR}/glew32${BUILD_SUFFIX}.lib")

add_library(glew::glew UNKNOWN IMPORTED)
set_target_properties(glew::glew PROPERTIES IMPORTED_LOCATION "${GLEW_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
