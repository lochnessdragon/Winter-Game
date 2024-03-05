# provide build instructions for imgui
cmake_minimum_required(VERSION 3.11)

include(FetchContent)

if(DEFINED INSTALL_DEPS)
	message(STATUS "Downloading imgui...")
	FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG origin/docking)
	FetchContent_MakeAvailable(imgui)
	message(STATUS "Downloaded imgui.")
	set(IMGUI_DIR ${imgui_SOURCE_DIR})
else()
	if( NOT DEFINED IMGUI_DIR )
	    # error out
        message(SEND_ERROR "Imgui couldn't be found, set the IMGUI_DIR variable to the location of the imgui src tree. Or, add -DINSTALL_DEPS=1 to the cmake args for cmake to automatically install the library.")
    endif()
endif()
string(REPLACE "\\" "/" IMGUI_DIR ${IMGUI_DIR})

project(imgui)

set(IMGUI_SOURCES ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# link OGL and GLFW
if(CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
	# tell emscripten to link glfw
	target_link_options(Engine PUBLIC "-sUSE_GLFW=3" "-sMAX_WEBGL_VERSION=2")
else()
	target_link_libraries(imgui PUBLIC glfw)
endif()