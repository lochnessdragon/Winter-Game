cmake_minimum_required(VERSION 3.2)

project(Minecraft)
set (CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  message(STATUS "C++ compiler: Emscripten")
  set(CMAKE_CXX_COMPILER_ID "Emscripten")
else()
  message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  # tell emscripten to create an html file
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  # test if em_cache is configured
  message(STATUS "EM_CACHE = $ENV{EM_CACHE}")
endif()

add_subdirectory(engine/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Minecraft ${SOURCES})
target_link_libraries(Minecraft Engine)
include_directories(Minecraft "engine/include/")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  # tell emscripten to link glfw
  target_link_options(Minecraft PUBLIC "-sUSE_GLFW=3")
endif()

if( CMAKE_COMPILER_IS_GNUCC )
    # there have been some weird bugs before w/ cmake reporting gcc on windows.
    target_compile_options(Minecraft PRIVATE -Wall -Wextra)
endif()

if( MSVC )
    # extra debug flags
    string(REGEX REPLACE "/Z[iI7]" ""
         CMAKE_CXX_FLAGS_DEBUG
         "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
    string(REGEX REPLACE "/W[1234]" ""
         CMAKE_CXX_FLAGS_DEBUG
         "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")

    set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT "Minecraft")
    set_property(TARGET Minecraft PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message("Setting VS Working Dir to: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()