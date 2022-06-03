#文件搜索目录
include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})

#设置目标名称
set (TARGET_NAME JUCE)

#设置编译宏
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_compile_definitions (_DEBUG)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	add_compile_definitions (NDEBUG)
endif()
add_compile_definitions (JUCE_DISPLAY_SPLASH_SCREEN=0)
add_compile_definitions (JUCE_USE_DARK_SPLASH_SCREEN=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_audio_basics=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_audio_devices=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_audio_formats=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_audio_processors=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_audio_utils=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_core=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_cryptography=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_data_structures=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_events=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_graphics=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_gui_basics=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_gui_extra=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_opengl=1)
add_compile_definitions (JUCE_MODULE_AVAILABLE_juce_video=1)
add_compile_definitions (JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
add_compile_definitions (JUCE_STANDALONE_APPLICATION=1)
add_compile_definitions (JUCE_DLL_BUILD=1)
add_compile_definitions (JucePlugin_Build_VST=0)
add_compile_definitions (JucePlugin_Build_VST3=0)
add_compile_definitions (JucePlugin_Build_AU=0)
add_compile_definitions (JucePlugin_Build_AUv3=0)
add_compile_definitions (JucePlugin_Build_RTAS=0)
add_compile_definitions (JucePlugin_Build_AAX=0)
add_compile_definitions (JucePlugin_Build_Standalone=0)
add_compile_definitions (JucePlugin_Build_Unity=0)
add_compile_definitions (JUCE_DISABLE_DLL_ALLOCATORS=1)
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
		add_link_options (-mbig-obj)
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
		add_link_options (-mbig-obj)
	endif ()
endif()

#编译的文件
add_library (
	${TARGET_NAME} SHARED
	${JUCEHEADER_DIR}/JuceHeader.h
	${JUCEHEADER_DIR}/include_juce_analytics.cpp
	${JUCEHEADER_DIR}/include_juce_audio_basics.cpp
	${JUCEHEADER_DIR}/include_juce_audio_devices.cpp
	${JUCEHEADER_DIR}/include_juce_audio_formats.cpp
	${JUCEHEADER_DIR}/include_juce_audio_processors.cpp
	${JUCEHEADER_DIR}/include_juce_audio_utils.cpp
	${JUCEHEADER_DIR}/include_juce_box2d.cpp
	${JUCEHEADER_DIR}/include_juce_core.cpp
	${JUCEHEADER_DIR}/include_juce_cryptography.cpp
	${JUCEHEADER_DIR}/include_juce_data_structures.cpp
	${JUCEHEADER_DIR}/include_juce_dsp.cpp
	${JUCEHEADER_DIR}/include_juce_events.cpp
	${JUCEHEADER_DIR}/include_juce_graphics.cpp
	${JUCEHEADER_DIR}/include_juce_gui_basics.cpp
	${JUCEHEADER_DIR}/include_juce_gui_extra.cpp
	${JUCEHEADER_DIR}/include_juce_opengl.cpp
	${JUCEHEADER_DIR}/include_juce_osc.cpp
	${JUCEHEADER_DIR}/include_juce_product_unlocking.cpp
	${JUCEHEADER_DIR}/include_juce_video.cpp
)

#设置目标位置
set_target_properties (
	${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
)