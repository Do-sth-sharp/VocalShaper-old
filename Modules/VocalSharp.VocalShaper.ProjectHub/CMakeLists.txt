﻿#获取当前目录名
string (REGEX REPLACE ".*/\(.*\)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

#设置基本属性
cmake_minimum_required (VERSION 3.15)

project (${CURRENT_FOLDER} VERSION 0.0.1 LANGUAGES CXX C)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release"  CACHE STRING "Choose the type of build." FORCE)
endif (NOT DEFINED CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)#使用C++20
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)#使用C17
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")#MSVC下使用/MD和/MDd

#初始化库目录
if (NOT DEFINED OUT_DIR)
	set (OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Out)
endif ()
if (NOT DEFINED JUCEHEADER_DIR)
	set (JUCEHEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/JuceLibraryCode)
endif ()
if (NOT DEFINED JUCELIBRARY_DIR)
	set (JUCELIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/JUCE/modules)
endif ()
if (NOT DEFINED LIBJMODULE_DIR)
	set (LIBJMODULE_DIR ${CMAKE_SOURCE_DIR}/libJModule)
endif ()
if (NOT DEFINED VSLIBRARY_DIR)
	set (VSLIBRARY_DIR ${CMAKE_SOURCE_DIR}/libVocalShaper)
endif ()

#文件搜索目录
include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})
include_directories (${LIBJMODULE_DIR})
include_directories (${VSLIBRARY_DIR})

#使用libJModule库
add_subdirectory (${LIBJMODULE_DIR} libJModule)

#设置目标名称
set (TARGET_NAME ${CURRENT_FOLDER})

#设置编译宏
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_compile_definitions (_DEBUG)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	add_compile_definitions (NDEBUG)
endif()
add_compile_definitions (JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
add_compile_definitions (JUCE_DLL=1)
add_compile_definitions (JUCE_DISABLE_DLL_ALLOCATORS=1)
add_compile_definitions (_SILENCE_CXX20_CISO646_REMOVED_WARNING=1)
endif ()
add_compile_definitions (JUCE_MODAL_LOOPS_PERMITTED=1)

#设置编译器参数
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options (
			/permissive-#符合模式:是
			/GS#安全检查:启用安全检查
			/Zc:wchar_t#将WChar_t视为内置类型:是
			/Gm-#启用最小重新生成:否
			/sdl#SDL检查:是
			/fp:precise#浮点模型:精度
			/Zc:forScope#强制For循环范围中的合规性:是
			/Gd#调用约定:__cdecl
			/nologo#取消显示启动版权标志:是
			/bigobj#较大生成对象:是
		)
		add_link_options (
			/MANIFEST#生成清单:是
			/NXCOMPAT#数据执行保护(DEP):是
			/DYNAMICBASE#随机基址:是
			/LARGEADDRESSAWARE#启用大地址:是
			/SUBSYSTEM:WINDOWS#子系统:窗口
			/MANIFESTUAC:NO#启用用户账户控制:否
			/NOLOGO#取消显示启动版权标志:是
		)
	else ()
		add_compile_options (-fPIC)
		#add_link_options (-mbig-obj)
	endif ()
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options (
			/permissive-#符合模式:是
			/GS#安全检查:启用安全检查
			/GL#全程序优化:是
			/Gy#启用函数级链接:是
			/Zc:wchar_t#将WChar_t视为内置类型:是
			/Gm-#启用最小重新生成:否
			/sdl#SDL检查:是
			/fp:precise#浮点模型:精度
			/Zc:forScope#强制For循环范围中的合规性:是
			/Gd#调用约定:__cdecl
			/Oi#启用内部函数:是
			/nologo#取消显示启动版权标志:是
			/bigobj#较大生成对象:是
		)
		add_link_options (
			/MANIFEST#生成清单:是
			/LTCG:incremental#链接时间代码生成:使用快速链接时间代码生成
			/NXCOMPAT#数据执行保护(DEP):是
			/DYNAMICBASE#随机基址:是
			/LARGEADDRESSAWARE#启用大地址:是
			/OPT:REF#引用:是
			/SUBSYSTEM:WINDOWS#子系统:窗口
			/MANIFESTUAC:NO#启用用户账户控制:否
			/OPT:ICF#启用COMDAT折叠:是
			/NOLOGO#取消显示启动版权标志:是
		)
	else ()
		add_compile_options (-fPIC)
		#add_link_options (-mbig-obj)
	endif ()
endif()

#添加目录内所有源文件
aux_source_directory(. TARGET_SOURCE)

#编译的文件
add_library (
	${TARGET_NAME} SHARED
	${LIBJMODULE_SRC}
	${TARGET_SOURCE}
)

#链接库
target_link_libraries (
	${TARGET_NAME}
	JUCE
	libVocalShaper
)

#设置目标位置
set_target_properties (
	${TARGET_NAME}
    PROPERTIES
	PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
    LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
    RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
)

#添加依赖
add_dependencies (
	${TARGET_NAME}
	JUCE
	libVocalShaper
)
