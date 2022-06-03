cmake_minimum_required (VERSION 3.15)

project (VocalShaper VERSION 0.0.1 LANGUAGES CXX C)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug"  CACHE STRING "Choose the type of build." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)#使用C++20
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)#使用C17
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")#MSVC下使用/MD和/MDd

#设置项目常用文件夹
set (OUT_DIR ${CMAKE_SOURCE_DIR}/Out)
set (JMADF_DIR ${CMAKE_SOURCE_DIR}/JMADF)
set (RC_DIR ${CMAKE_SOURCE_DIR}/Resources)
set (LIBJMODULE_DIR ${JMADF_DIR}/libJModule)
set (JUCEHEADER_DIR ${CMAKE_SOURCE_DIR}/JuceLibraryCode)
set (JUCELIBRARY_DIR ${CMAKE_SOURCE_DIR}/JUCE_Modules)

#编译目标
add_subdirectory (Modules)
add_subdirectory (VocalShaper)
add_subdirectory (JUCE_Library)
