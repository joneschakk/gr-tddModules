find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_TDDMODULES gnuradio-tddModules)

FIND_PATH(
    GR_TDDMODULES_INCLUDE_DIRS
    NAMES gnuradio/tddModules/api.h
    HINTS $ENV{TDDMODULES_DIR}/include
        ${PC_TDDMODULES_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_TDDMODULES_LIBRARIES
    NAMES gnuradio-tddModules
    HINTS $ENV{TDDMODULES_DIR}/lib
        ${PC_TDDMODULES_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-tddModulesTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_TDDMODULES DEFAULT_MSG GR_TDDMODULES_LIBRARIES GR_TDDMODULES_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_TDDMODULES_LIBRARIES GR_TDDMODULES_INCLUDE_DIRS)
