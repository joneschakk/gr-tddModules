find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_TDDMODULEPKG gnuradio-tddModulePkg)

FIND_PATH(
    GR_TDDMODULEPKG_INCLUDE_DIRS
    NAMES gnuradio/tddModulePkg/api.h
    HINTS $ENV{TDDMODULEPKG_DIR}/include
        ${PC_TDDMODULEPKG_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_TDDMODULEPKG_LIBRARIES
    NAMES gnuradio-tddModulePkg
    HINTS $ENV{TDDMODULEPKG_DIR}/lib
        ${PC_TDDMODULEPKG_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-tddModulePkgTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_TDDMODULEPKG DEFAULT_MSG GR_TDDMODULEPKG_LIBRARIES GR_TDDMODULEPKG_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_TDDMODULEPKG_LIBRARIES GR_TDDMODULEPKG_INCLUDE_DIRS)
