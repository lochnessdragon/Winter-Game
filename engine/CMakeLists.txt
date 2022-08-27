cmake_minimum_required(VERSION 3.2)

project(Engine)

add_subdirectory(lib/glfw/)

add_subdirectory(lib/glm/)

add_subdirectory(lib/glad/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Engine STATIC ${SOURCES})
target_link_libraries(Engine glfw Glad glm GL)
target_include_directories(Engine PUBLIC "include/" "lib/glfw/include" "lib/glad/include" "lib/glm" "lib/stb")

if( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(Engine PRIVATE -Wall -Wextra)
endif()

if( MSVC )
    target_compile_options(Engine PRIVATE /W4)
endif()