cmake_minimum_required(VERSION 3.2)

project(Engine)

function(CheckDep depName pkgName)
  find_package(${pkgName})
  if (NOT ${pkgName}_FOUND)
    if(DEFINED INSTALL_DEPS)
      message(STATUS "Checking if ${depName} is installed.")
    elseif( NOT DEFINED ${depName}_DIR )
      # error out
      message(SEND_ERROR "${depName} couldn't be found, set the ${depName}_DIR variable to the location of the ${depName} src tree. Or, add -DINSTALL_DEPS=1 to the cmake args for cmake to automatically install the library.")
    endif()
  else()
    message(STATUS "Found package: ${depName}")
  endif()
endfunction()

function(AddDep targetName depName pkgName libName gitURL)
  # we need the name of the variable set for the library source directory
  # we need the find_package name
  # we need the target name
  # we need a git source repo to download the file from
  # we need to know whether to link the library or just add the header

  find_package(${pkgName})
  if (NOT ${pkgName}_FOUND)
    if(DEFINED INSTALL_DEPS)
      message(STATUS "Installing: ${depName}.")
      # install the dependency
      
    else()
      if( NOT DEFINED ${depName}_DIR )
        # error out
        message(SEND_ERROR "${depName} couldn't be found, set the ${depName}_DIR variable to the location of the ${depName} src tree. Or, add -DINSTALL_DEPS=1 to the cmake args for cmake to automatically install the library.")
      else()
        # add the file as a subdirectory
        add_subdirectory(${${depName}_DIR} lib/${depName})
      endif()
    endif()
  else()
    message(STATUS "Found package: ${depName}")
    # alright, add the target as a linked file
    target_link_libraries(${targetName} PUBLIC ${libName})
  endif()
endfunction()

add_subdirectory(lib/glad/)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Engine STATIC ${SOURCES})
# add dependencies
AddDep(Engine GLFW glfw3 glfw)
AddDep(Engine GLM glm glm)
AddDep(Engine ENTT EnTT EnTT::EnTT)
AddDep(Engine SPDLOG spdlog spdlog)
AddDep(Engine STB stb stb)

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
