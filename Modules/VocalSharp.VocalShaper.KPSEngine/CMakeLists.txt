#获取当前目录名
string (REGEX REPLACE ".*/\(.*\)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

#启用CUDA
set (TARGET_CUDA_ENABLED OFF)

#准备CUDA
if (TARGET_CUDA_ENABLED)
	find_package (CUDA)
	if (NOT CUDA_FOUND)
		message ("CUDA DISABLED!!! NOT FOUND!!!")
		set (TARGET_CUDA_ENABLED OFF)
	endif (NOT CUDA_FOUND)
endif (TARGET_CUDA_ENABLED)

#设置基本属性
cmake_minimum_required (VERSION 3.23)

project (${CURRENT_FOLDER} VERSION 0.0.1 LANGUAGES CXX C)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release"  CACHE STRING "Choose the type of build." FORCE)
endif (NOT DEFINED CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 20)#使用C++20
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)#使用C17
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")#MSVC下使用/MD和/MDd

#CUDA配置
if (TARGET_CUDA_ENABLED)
	enable_language (CUDA)
	set (CMAKE_CUDA_STANDARD 17)#使用C++17
	set (CMAKE_CUDA_EXTENSIONS OFF)
	set (CMAKE_CUDA_STANDARD_REQUIRED ON)
	set (CMAKE_CUDA_SEPARABLE_COMPILATION ON)
	set (CMAKE_CUDA_ARCHITECTURES all)
endif (TARGET_CUDA_ENABLED)

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
if (TARGET_CUDA_ENABLED)
	include_directories (${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
endif (TARGET_CUDA_ENABLED)

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
if (TARGET_CUDA_ENABLED)
	add_compile_definitions (CUDA_ENABLED=1)
endif (TARGET_CUDA_ENABLED)
add_compile_definitions (JUCE_MODAL_LOOPS_PERMITTED=1)

#添加目录内所有源文件
aux_source_directory (. TARGET_SOURCE)
aux_source_directory (algorithm TARGET_ALGORITHM_SOURCE)

#添加CUDA文件
if (TARGET_CUDA_ENABLED)
	aux_source_directory (cuda TARGET_CUDA_SOURCE)
	aux_source_directory (cuda/test TARGET_CUDA_TEST_SOURCE)
endif (TARGET_CUDA_ENABLED)

#编译的文件
add_library (
	${TARGET_NAME} SHARED
	${LIBJMODULE_SRC}
	${TARGET_SOURCE}
	${TARGET_CUDA_SOURCE}
	${TARGET_ALGORITHM_SOURCE}
)

#设置编译器参数
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		#set (CMAKE_CXX_FLAGS "/bigobj" ${CMAKE_CXX_FLAGS})
		set_target_properties(
			${TARGET_NAME} PROPERTIES
			LINK_FLAGS 
			/MANIFEST #生成清单:是
			/NXCOMPAT #数据执行保护(DEP):是
			/DYNAMICBASE #随机基址:是
			/LARGEADDRESSAWARE #启用大地址:是
			/SUBSYSTEM:WINDOWS #子系统:窗口
			/MANIFESTUAC:NO #启用用户账户控制:否
			/NOLOGO #取消显示启动版权标志:是
		)
	else ()
		set (CMAKE_CXX_FLAGS -fPIC ${CMAKE_CXX_FLAGS})
		#add_link_options (-mbig-obj)
	endif ()
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		#set (CMAKE_CXX_FLAGS "/bigobj" ${CMAKE_CXX_FLAGS})
		set_target_properties(
			${TARGET_NAME} PROPERTIES
			LINK_FLAGS 
			/MANIFEST #生成清单:是
			/LTCG:incremental #链接时间代码生成:使用快速链接时间代码生成
			/NXCOMPAT #数据执行保护(DEP):是
			/DYNAMICBASE #随机基址:是
			/LARGEADDRESSAWARE #启用大地址:是
			/OPT:REF #引用:是
			/SUBSYSTEM:WINDOWS #子系统:窗口
			/MANIFESTUAC:NO #启用用户账户控制:否
			/OPT:ICF #启用COMDAT折叠:是
			/NOLOGO #取消显示启动版权标志:是
		)
	else ()
		set (CMAKE_CXX_FLAGS -fPIC ${CMAKE_CXX_FLAGS})
		#add_link_options (-mbig-obj)
	endif ()
endif()

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

#测试目标
if (TARGET_CUDA_ENABLED)
	add_executable (
		KarPlusStrong_Cuda_Test
		${TARGET_CUDA_SOURCE}
		${TARGET_CUDA_TEST_SOURCE}
		${TARGET_ALGORITHM_SOURCE}
	)

	set_target_properties (
		KarPlusStrong_Cuda_Test
	    PROPERTIES
		PREFIX ""
	    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
	    LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
	    RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}/modules/${TARGET_NAME}
	)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set_target_properties(
			KarPlusStrong_Cuda_Test PROPERTIES
			LINK_FLAGS 
			/SUBSYSTEM:CONSOLE #子系统:控制台
		)
	endif ()

endif (TARGET_CUDA_ENABLED)