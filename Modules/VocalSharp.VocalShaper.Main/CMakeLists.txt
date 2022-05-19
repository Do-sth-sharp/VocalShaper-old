aux_source_directory (. MODULE_SRC)
aux_source_directory (${LIBJMODULE_DIR} MODULE_SRC)

include_directories (${CMAKE_SOURCE_DIR})
include_directories (${JUCEHEADER_DIR})
include_directories (${JUCELIBRARY_DIR})

add_library (VocalSharp.VocalShaper.Main SHARED ${MODULE_SRC})