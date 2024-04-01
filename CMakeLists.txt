set(COMPILE_ID ovr_tracking_native)

cmake_minimum_required(VERSION 3.27)
project(${COMPILE_ID})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/native/)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    message(SEND_ERROR "Compiler is not supported. Please use Clang.")
endif ()

MACRO(RECURSE_FILES return_list filter)
    FILE(GLOB_RECURSE new_list ${filter})
    SET(file_list "")
    FOREACH(file_path ${new_list})
        SET(file_list ${file_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()

if (MELONSEED_INSTALL_AT_GAME)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAME_PATH}/MelonSeed)
endif ()

RECURSE_FILES(cpp_file_list ${SOURCE_DIR}/*.cpp)
RECURSE_FILES(c_file_list ${SOURCE_DIR}/*.c)

add_library(
        ${COMPILE_ID}
        SHARED
        ${cpp_file_list}
        ${c_file_list}
)

# static here means we dont need to include libc++
target_link_options(${COMPILE_ID} PUBLIC -static)

target_link_libraries(${COMPILE_ID} PUBLIC "${CMAKE_SOURCE_DIR}/openvr/bin/win64/libopenvr_api64.a")
add_compile_definitions(-DOPENVR_BUILD_STATIC)

target_include_directories(${COMPILE_ID} PRIVATE .)
target_include_directories(${COMPILE_ID} PRIVATE ${SOURCE_DIR})

target_include_directories(${COMPILE_ID} PUBLIC openvr/headers)