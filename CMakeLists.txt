cmake_minimum_required(VERSION 3.20)
project(setproxy)


# Enable "CMakePredefinedTargets" folder in IDEs.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# Enable output of compile commands during generation.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Enable debugging for "ALL_BUILD" project.
if (MSVC AND NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
    file(GENERATE
        OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
        INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
endif()


add_subdirectory(src)
