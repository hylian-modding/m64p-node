set(EXTERNALS_DIR "${CMAKE_SOURCE_DIR}/externals")

add_library(fmt::fmt INTERFACE IMPORTED)
set_target_properties(fmt::fmt PROPERTIES INTERFACE_COMPILE_DEFINITIONS "FMT_HEADER_ONLY=1"
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_DIR}/fmt-7.1.3/include")

add_library(nano-signal-slot::nano-signal-slot INTERFACE IMPORTED)
set_target_properties(nano-signal-slot::nano-signal-slot PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_DIR}/nano-signal-slot-2.0.1/include")

add_library(stb::stb INTERFACE IMPORTED)
set_target_properties(stb::stb PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_DIR}/stb-b42009b/include")

add_library(utfcpp::utfcpp INTERFACE IMPORTED)
set_target_properties(utfcpp::utfcpp PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_DIR}/utfcpp-3.1.2/include")

add_library(m64p::m64p-api INTERFACE IMPORTED)
set_target_properties(m64p::m64p-api PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EXTERNALS_DIR}/m64p-api/include")
