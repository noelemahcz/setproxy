add_executable(setproxy main.cpp)


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(setproxy PRIVATE /utf-8)
    set_target_properties(setproxy PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set_target_properties(setproxy PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(setproxy PRIVATE -municode)
endif()


# For windows sdk and microsoft c runtime library.
target_compile_definitions(setproxy PRIVATE
    _UNICODE
    UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX)


target_link_libraries(setproxy PRIVATE Wininet)
