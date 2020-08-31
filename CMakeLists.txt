cmake_minimum_required(VERSION 3.9)

project(ml64_emu_addon)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include(ImportNodeTargets)
include(ImportExternalTargets)

if(WIN32)
    include(ImportWindowsTargets)
elseif(UNIX)
    include(ImportLinuxTargets)
    add_compile_options(-Wall -Wextra)
endif()

include_directories(src)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_subdirectory(src)
