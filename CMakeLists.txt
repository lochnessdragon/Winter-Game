cmake_minimum_required(VERSION 3.11)

include (cmake/vs_source_groups.cmake)

project(WinterGame)
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

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(WinterGame ${SOURCES})
target_link_libraries(WinterGame Engine)
include_directories(WinterGame "engine/include/")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  # tell emscripten to link glfw
  # target_compile_options(WinterGame PUBLIC "--preload-file res@/res")
  target_link_options(WinterGame PUBLIC "-sUSE_GLFW=3" "-sMAX_WEBGL_VERSION=2" "--preload-file" "../res@res")
endif()

if( CMAKE_COMPILER_IS_GNUCC )
    # there have been some weird bugs before w/ cmake reporting gcc on windows.
    target_compile_options(WinterGame PRIVATE -Wall -Wextra)
endif()

if( MSVC )
    # extra debug flags
    string(REGEX REPLACE "/Z[iI7]" ""
         CMAKE_CXX_FLAGS_DEBUG
         "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
    # string(REGEX REPLACE "/W[1234]" ""
    #     CMAKE_CXX_FLAGS_DEBUG
    #     "${CMAKE_CXX_FLAGS_DEBUG}")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")

    set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT "WinterGame")
    set_property(TARGET WinterGame PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message("Setting VS Working Dir to: ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if( APPLE )
    set_target_properties(WinterGame PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# copy over OpenAL32.dll
add_custom_command(TARGET WinterGame
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenAL_BINARY_DIR}/$<CONFIGURATION>/OpenAL32.dll ${WinterGame_BINARY_DIR}/$<CONFIGURATION>/.
)
