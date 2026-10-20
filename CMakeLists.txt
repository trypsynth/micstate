cmake_minimum_required(VERSION 3.21)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/bin/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_MANIFEST_DIR "${CMAKE_SOURCE_DIR}/vcpkg")
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "Vcpkg triplet")
project(micstate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC AND VCPKG_TARGET_TRIPLET MATCHES "-static$")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")
find_package(prism CONFIG REQUIRED)

file(GLOB SRCS "src/*.cpp")
add_executable(micstate WIN32 ${SRCS})
target_include_directories(micstate PRIVATE ${MINIAUDIO_INCLUDE_DIRS})
target_link_libraries(micstate PRIVATE Kernel32 Psapi prism::prism Version winmm)

file(GLOB DLLS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*.dll")
set(LOCAL_DLLS "")
foreach(DLL IN LISTS DLLS)
	get_filename_component(DLL_NAME "${DLL}" NAME)
	add_custom_command(TARGET micstate POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL}" "$<TARGET_FILE_DIR:micstate>")
	list(APPEND LOCAL_DLLS "${DLL_NAME}")
endforeach()
