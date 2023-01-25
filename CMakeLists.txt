cmake_minimum_required(VERSION 3.2)

project(Minecraft)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(engine/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Minecraft ${SOURCES})
target_link_libraries(Minecraft Engine)
include_directories(Minecraft "engine/include/")

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