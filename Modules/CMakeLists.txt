#获取模块并包含
macro (SUBDIRLIST result curdir)
  file (GLOB children RELATIVE ${curdir} ${curdir}/*)
  set (dirlist "")
  foreach (child ${children})
    if (IS_DIRECTORY ${curdir}/${child})
      list (APPEND dirlist ${child})
    endif ()
  endforeach ()
  set (${result} ${dirlist})
endmacro ()

SUBDIRLIST (SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach (subdir ${SUBDIRS})
  add_subdirectory (${subdir})
endforeach ()

#导出模块列表
set (MODULE_LIST ${SUBDIRS} CACHE STRING INTERNAL)
