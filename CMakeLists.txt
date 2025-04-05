cmake_minimum_required(VERSION 3.20)
project(MicState LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
link_directories(lib)
file(GLOB SRCS "src/*.cpp")

add_executable(micstate WIN32 ${SRCS})
target_link_libraries(micstate PRIVATE Kernel32 Psapi UniversalSpeechStatic Version winmm)
set_target_properties(micstate PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
