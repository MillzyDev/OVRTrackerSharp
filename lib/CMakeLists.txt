set(COMPILE_ID TrackerTransmitter)

cmake_minimum_required(VERSION 3.27)
project(${COMPILE_ID})

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)

MACRO(RECURSE_FILES return_list filter)
    FILE(GLOB_RECURSE new_list ${filter})
    SET(file_list "")
    FOREACH(file_path ${new_list})
        SET(file_list ${file_list} ${file_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES file_list)
    SET(${return_list} ${file_list})
ENDMACRO()

RECURSE_FILES(cpp_file_list ${SOURCE_DIR}/*.cpp)
RECURSE_FILES(c_file_list ${SOURCE_DIR}/*.c)

if (${TT_STATIC})
    add_library(
        ${COMPILE_ID}
        STATIC
        ${cpp_file_list}
        ${c_file_list}
    )
else ()
    add_library(
        ${COMPILE_ID}
        SHARED
        ${cpp_file_list}
        ${c_file_list}
    )
endif ()

# static here means we dont need to include libc
target_link_options(${COMPILE_ID} PUBLIC -static)

target_include_directories(${COMPILE_ID} PRIVATE .)
target_include_directories(${COMPILE_ID} PRIVATE ${SOURCE_DIR})
target_include_directories(${COMPILE_ID} PRIVATE ${INCLUDE_DIR})
target_include_directories(${COMPILE_ID} PUBLIC ${SHARED_DIR})

target_compile_definitions(${COMPILE_ID} PUBLIC TT_EXPORTS)