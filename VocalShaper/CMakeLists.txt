#文件搜索目录
include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})
include_directories (${JMADF_DIR})
include_directories (${LIBJMODULE_DIR})
include_directories (${RC_DIR})

#使用JMADF库
add_subdirectory (${JMADF_DIR} JMADF)

#设置目标名称
set (TARGET_NAME VocalShaper)

#设置编译宏
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_compile_definitions (_DEBUG)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	add_compile_definitions (NDEBUG)
endif()
add_compile_definitions (JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
add_compile_definitions (JUCE_DLL=1)
add_compile_definitions (JUCE_DISABLE_DLL_ALLOCATORS=1)
add_compile_definitions (JUCE_MODAL_LOOPS_PERMITTED=1)

#设置编译器参数
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options (
			/GS#安全检查:启用安全检查
			/Zc:wchar_t#将WChar_t视为内置类型:是
			/Gm-#启用最小重新生成:否
			/sdl#SDL检查:是
			/fp:precise#浮点模型:精度
			/Zc:forScope#强制For循环范围中的合规性:是
			/GR#启用运行时类型信息:是
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
		add_link_options (-mbig-obj)
	endif ()
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options (
			/GS#安全检查:启用安全检查
			/GL#全程序优化:是
			/Gy#启用函数级链接:是
			/Zc:wchar_t#将WChar_t视为内置类型:是
			/Gm-#启用最小重新生成:否
			/sdl#SDL检查:是
			/fp:precise#浮点模型:精度
			/Zc:forScope#强制For循环范围中的合规性:是
			/GR#启用运行时类型信息:是
			/Gd#调用约定:__cdecl
			/nologo#取消显示启动版权标志:是
			/bigobj#较大生成对象:是
		)
		add_link_options (
			/MANIFEST#生成清单:是
			/LTCG#链接时间代码生成:使用链接时间代码生成
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
		add_link_options (-mbig-obj)
	endif ()
endif()

#编译的文件
add_executable (
	${PROJECT_NAME}
	${JMADF_SRC}
	${RC_DIR}/resources.rc
	${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Splash.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Splash.h
)

#链接库
target_link_libraries (
	${TARGET_NAME}
	JUCE
)

#设置目标位置
set_target_properties (
	${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
)

#添加依赖
add_dependencies (
	${TARGET_NAME}
	JUCE
	VocalSharp.VocalShaper.Main
	VocalSharp.VocalShaper.MainWindow
)
