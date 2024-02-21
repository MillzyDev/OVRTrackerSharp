set(COMPILE_ID TrackerTransmitter)

cmake_minimum_required(VERSION 3.27)
project(${COMPILE_ID})

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

# Check if 32 or 64 bit system.
set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PROCESSOR_ARCH "64")
else()
    set(PROCESSOR_ARCH "32")
endif()

# Get platform.
if(WIN32)
    set(PLATFORM_NAME "win")
    if(NOT BUILD_SHARED)
        add_definitions(-DOPENVR_BUILD_STATIC)
    endif()
elseif(UNIX AND NOT APPLE)
    if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
        set(PLATFORM_NAME "linux")
        add_definitions(-DLINUX -DPOSIX)
        if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
            add_definitions(-DLINUXARM64)
        elseif(PROCESSOR_ARCH MATCHES "64")
            add_definitions(-DLINUX64)
        endif()
    endif()
elseif(APPLE)
    if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*" OR CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
        set(PLATFORM_NAME "osx")
        add_definitions(-DOSX -DPOSIX)
        if(BUILD_UNIVERSAL)
            set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
        endif()
        if(BUILD_OSX_I386)
            set(PROCESSOR_ARCH "32")
            set(CMAKE_OSX_ARCHITECTURES "i386")
        endif()
    endif()
endif()

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

# static here means we dont need to include libs
target_link_options(${COMPILE_ID} PUBLIC -static)

target_include_directories(${COMPILE_ID} PRIVATE .)
target_include_directories(${COMPILE_ID} PRIVATE ${SOURCE_DIR})
target_include_directories(${COMPILE_ID} PRIVATE ${INCLUDE_DIR})
target_include_directories(${COMPILE_ID} PUBLIC ${SHARED_DIR})

target_compile_definitions(${COMPILE_ID} PUBLIC TT_EXPORTS)

target_include_directories(${COMPILE_ID} PRIVATE openvr/headers)

if (${PROCESSOR_ARCH} STREQUAL "64")
    set(OPENVR_SUFFIX "64")
endif ()

if (WIN32)
    if (MSVC)
        set(OPENVR_LINK_LIBRARY_NAME "openvr_api${OPENVR_SUFFIX}.lib")
    else()
        set(OPENVR_LINK_LIBRARY_NAME "openvr_api${OPENVR_SUFFIX}.dll")
    endif ()
elseif (UNIX AND NOT APPLE)
    set(OPENVR_LINK_LIBRARY_NAME "libopenvr_api${OPENVR_SUFFIX}.so")
elseif (APPLE)
    set(OPENVR_LINK_LIBRARY_NAME "libopenvr_api${OPENVR_SUFFIX}.dylib")
endif ()

set(OPENVR_BINARIES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/openvr/bin")
set(OPENVR_TARGET_BINARY "${OPENVR_BINARIES_PATH}/${PLATFORM_NAME}${PROCESSOR_ARCH}/${OPENVR_LINK_LIBRARY_NAME}")

target_link_libraries(${COMPILE_ID} ${OPENVR_TARGET_BINARY})