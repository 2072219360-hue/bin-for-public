function(dev701_import_library target default_implib)
    set(cache_name "DEV701_${target}_IMPLIB")
    set(${cache_name} "${default_implib}" CACHE FILEPATH
        "Debug x64 import/static library for ${target}")
    if(NOT EXISTS "${${cache_name}}")
        message(FATAL_ERROR "${target} library not found: ${${cache_name}}")
    endif()
    add_library(${target} UNKNOWN IMPORTED GLOBAL)
    set_target_properties(${target} PROPERTIES
        IMPORTED_LOCATION "${${cache_name}}"
        IMPORTED_CONFIGURATIONS Debug)
endfunction()

# These filenames preserve the known Debug x64 .vcxproj link contract. They
# are intentionally cache variables so a clean machine can provide verified
# replacements without aliases, junctions, or fixed drive letters.
dev701_import_library(legacy_qwt "${PROJECT_SOURCE_DIR}/lib/huangqwtd.lib")
dev701_import_library(legacy_qwtplot3d "${PROJECT_SOURCE_DIR}/lib/huangqwtplot3d.lib")
dev701_import_library(legacy_qglviewer "${PROJECT_SOURCE_DIR}/lib/HuangzbQGLVastyellowd2.lib")
dev701_import_library(legacy_qssh "${PROJECT_SOURCE_DIR}/lib/QSshd.lib")
dev701_import_library(legacy_jsoncpp "${PROJECT_SOURCE_DIR}/lib/jsoncpp.lib")
dev701_import_library(legacy_jsoncpp_static "${PROJECT_SOURCE_DIR}/lib/jsoncpp_static.lib")
