cmake_minimum_required(VERSION 3.2)

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
        target_include_directories(${targetName} PUBLIC ${${pkgName}_SOURCE_DIR})
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
		else()
		  target_include_directories(${targetName} PUBLIC "${${depName}_DIR}/")
		endif()
      endif()
    endif()
  else()
    message(STATUS "Found package: ${depName}")
    # alright, add the target as a linked file
    target_link_libraries(${targetName} PUBLIC ${libName})
  endif()
endfunction()

add_subdirectory(lib/glad/)

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

add_library(Engine STATIC ${SOURCES})
# add dependencies
if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  # tell emscripten to link glfw and webgl2
  target_link_options(Engine PUBLIC "-sUSE_GLFW=3" "-sMAX_WEBGL_VERSION=2")
else()
  AddDep(Engine GLFW glfw3 glfw 0 https://github.com/glfw/glfw.git 57cbded0760a50b9039ee0cb3f3c14f60145567c)
endif()

AddDep(Engine GLM glm glm::glm 0 https://github.com/g-truc/glm.git efec5db081e3aad807d0731e172ac597f6a39447)
AddDep(Engine ENTT EnTT EnTT::EnTT 0 https://github.com/skypjack/entt.git e48817d518c0ada57665be07ba9961c19f71b502)
AddDep(Engine SPDLOG spdlog spdlog 0 https://github.com/gabime/spdlog.git 5a589438d2d2abc67bdbec2aad7e2a94800c00ea)
AddDep(Engine STB stb stb 1 https://github.com/nothings/stb.git 8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55)

# if we are on replit then we are going to have to 
# link libfmt as well
#if(DEFINED ENV{REPL_ID})
#  target_link_libraries(Engine PUBLIC fmt)
#endif()

# link glad
target_link_libraries(Engine PRIVATE Glad)

# setup includes
target_include_directories(Engine PUBLIC "include/" "lib/glad/include")

if( CMAKE_COMPILER_IS_GNUCC )
    message(STATUS "Linux build detected: " ${CMAKE_CXX_COMPILER_ID})

    target_compile_options(Engine PRIVATE -Wall -Wextra)
    target_link_libraries(Engine PUBLIC GL)
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

    #target_compile_options(Engine PRIVATE /W4 /ZI)
endif()
