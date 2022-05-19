#文件搜索目录
include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})

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

#编译的文件
add_library (
	libJUCE SHARED
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

#较大生成对象
target_compile_options(
	libJUCE
	PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/bigobj> 
	$<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>
)

#设置目标位置
set_target_properties(
	libJUCE
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${OUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}"
)