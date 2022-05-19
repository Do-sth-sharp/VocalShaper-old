include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})
aux_source_directory (${JMADF_DIR} JMADF_SRC)

add_executable (
	${PROJECT_NAME}
	Main.cpp
	Splash.cpp
	Splash.h
	${JMADF_SRC}
)
