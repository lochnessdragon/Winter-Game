cmake_minimum_required(VERSION 3.11)

include(FetchContent)

project(Engine)

function(AddDep targetName depName pkgName libName headerOnly gitUrl gitTag)
  # we need the name of the variable set for the library source directory
  # we need the find_package name
  # we need the target name
  # we need a git source repo to download the file from
  # we need to know whether to link the library or just add the header
  # now, emscripten provides a difficult challenge for dependency management
  # glfw is included in emscripten whilst all the others are not.
  # therefore we need to avoid installing glfw but we will be FORCED to install the others
  find_package(${pkgName} QUIET)
  if (NOT ${pkgName}_FOUND)
    if(DEFINED INSTALL_DEPS)
      message(STATUS "Installing: ${depName}...")
      # install the dependency
      FetchContent_Declare(
        ${pkgName}
        GIT_REPOSITORY ${gitUrl}
        GIT_TAG ${gitTag}
      )

      FetchContent_MakeAvailable(${pkgName})

      if(headerOnly)
        # only include the file
        target_include_directories(${targetName} SYSTEM PUBLIC ${${pkgName}_SOURCE_DIR})
      else()
        # link the library
        target_link_libraries(${targetName} PUBLIC ${libName})
      endif()
      message(STATUS "Installed: ${depName}")
    else()
      if( NOT DEFINED ${depName}_DIR )
        # error out
        message(SEND_ERROR "${depName} couldn't be found, set the ${depName}_DIR variable to the location of the ${depName} src tree. Or, add -DINSTALL_DEPS=1 to the cmake args for cmake to automatically install the library.")
      else()
        # add the file as a subdirectory
		if( NOT headerOnly )
		  add_subdirectory(${${depName}_DIR} lib/${depName})
		  target_link_libraries(${targetName} PUBLIC ${libName})
          message(STATUS "Added ${depName} as a subdirectory and linked ${libName}")
		else()
		  target_include_directories(${targetName} SYSTEM PUBLIC "${${depName}_DIR}/")
		endif()
      endif()
    endif()
  else()
    message(STATUS "Located depedency (using find_package): ${depName}")
    # alright, add the target as a linked file
    target_link_libraries(${targetName} PUBLIC ${libName})
  endif()
endfunction()

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
	add_subdirectory(lib/glad/)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

add_library(Engine STATIC ${SOURCES})
# add dependencies
if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  # tell emscripten to link glfw and webgl2
  target_compile_options(Engine PUBLIC "-fwasm-exceptions")
  target_link_options(Engine PUBLIC "-sUSE_GLFW=3" "-sMAX_WEBGL_VERSION=2" "-fwasm-exceptions")
else()
  AddDep(Engine GLFW glfw3 glfw 0 https://github.com/glfw/glfw.git 57cbded0760a50b9039ee0cb3f3c14f60145567c)
endif()

AddDep(Engine GLM glm glm::glm 0 https://github.com/g-truc/glm.git efec5db081e3aad807d0731e172ac597f6a39447)
# AddDep(Engine ENTT EnTT EnTT::EnTT 0 https://github.com/skypjack/entt.git e48817d518c0ada57665be07ba9961c19f71b502) remove entt
AddDep(Engine SPDLOG spdlog spdlog 0 https://github.com/gabime/spdlog.git 5a589438d2d2abc67bdbec2aad7e2a94800c00ea)
AddDep(Engine STB stb stb 1 https://github.com/nothings/stb.git 8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55)
AddDep(Engine NLOHMANN_JSON nlohmann_json nlohmann_json::nlohmann_json 0 https://github.com/nlohmann/json.git develop)
AddDep(Engine OPENAL_SOFT OpenAL OpenAL 0 https://github.com/kcat/openal-soft.git master)

# ImGUI
include(cmake/imgui.cmake)
target_link_libraries(Engine PUBLIC imgui)

# disable non-requisite free type libraries:
# // Disable support of compressed WOFF2 fonts.
# FT_DISABLE_BROTLI:BOOL=OFF
# // Disable support of bzip2 compressed fonts.
# FT_DISABLE_BZIP2:BOOL=OFF
# // Disable HarfBuzz (used for improving auto-hinting of OpenType fonts).
# FT_DISABLE_HARFBUZZ:BOOL=OFF
# // Disable support of PNG compressed OpenType embedded bitmaps.
# FT_DISABLE_PNG:BOOL=OFF
# // Disable use of system zlib and use internal zlib library instead.
# FT_DISABLE_ZLIB:BOOL=OFF
SET(FT_DISABLE_BROTLI ON CACHE INTERNAL "Disable support of compressed WOFF2 fonts." FORCE)
SET(FT_DISABLE_BZIP2 ON CACHE INTERNAL "Disable support of bzip2 compressed fonts." FORCE)
SET(FT_DISABLE_HARFBUZZ ON CACHE INTERNAL "Disable HarfBuzz (used for improving auto-hinting of OpenType fonts)." FORCE)
SET(FT_DISABLE_PNG ON CACHE INTERNAL "Disable support of PNG compressed OpenType embedded bitmaps." FORCE)
SET(FT_DISABLE_ZLIB ON CACHE INTERNAL "Disable use of system zlib and use internal zlib library instead." FORCE)
AddDep(Engine FREETYPE Freetype freetype 0 https://gitlab.freedesktop.org/freetype/freetype.git 4e61303a3b0e6656c38dd9da9f70d18d7e30585b)

if(TARGET Freetype::Freetype)
    message(STATUS "Found freetype library")
endif()

# MSDF Gen Specific Settings
# Look at MTSDF for soft effects as well (potentially)
set(MSDF_ATLAS_NO_ARTERY_FONT ON CACHE INTERNAL "Disable Artery Font export and do not require its submodule" FORCE)
set(MSDF_ATLAS_USE_SKIA OFF CACHE INTERNAL "Build with the Skia library" FORCE)
set(MSDF_ATLAS_USE_VCPKG OFF CACHE INTERNAL "Use vcpkg package manager to link project dependencies" FORCE)
set(MSDF_ATLAS_STRIPPED ON CACHE INTERNAL "Enable font loading ONLY (i.e. remove dependency on lib png and tinyxml2" FORCE)
set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE INTERNAL "Build the msdf-atlas-gen standalone executable" FORCE)
set(MSDFGEN_FREETYPE_SUBDIRECTORY ON CACHE INTERNAL "Freetype has been included as a subdirectory" FORCE)
AddDep(Engine MSDF_ATLAS_GEN msdf-atlas-gen msdf-atlas-gen 0 https://github.com/Chlumsky/msdf-atlas-gen.git 3cf874b39a2ea0acd316651cf10f5d13f75371fd)

# if we are on replit then we are going to have to 
# link libfmt as well
#if(DEFINED ENV{REPL_ID})
#  target_link_libraries(Engine PUBLIC fmt)
#endif()

# link glad (but not for the web)
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
	target_link_libraries(Engine PRIVATE Glad)
	target_include_directories(Engine PUBLIC "lib/glad/include")
endif()

# include par_shapes
target_include_directories(Engine PRIVATE "lib/par_shapes")

# setup includes
target_include_directories(Engine PUBLIC "include/")

# Extra linux compiler options
if( CMAKE_COMPILER_IS_GNUCC )
    message(STATUS "Linux build detected: " ${CMAKE_CXX_COMPILER_ID})

    target_compile_options(Engine PRIVATE -Wall -Wextra)
    target_link_libraries(Engine PUBLIC GL)
endif()

# Extra visual studio project settings
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

    # count includes with angle brackets as external to the project (don't warn about them)
    # TODO: fix warnings on spdlog library
    target_compile_options(Engine PRIVATE /external:anglebrackets)

    # force headers and source files into directory tree in visual studio
    assign_source_group(${SOURCES})
endif()
