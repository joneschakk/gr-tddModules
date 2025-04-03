# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT tddModulePkg_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT tddModulePkg_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND tddModulePkg_python_files
    tddRandomSrc_python.cc
    tddTaggedStreamMux_python.cc
    tddOFDMCyclicPrefix_python.cc
    tddMessageStrobe_python.cc
    tddLatReadSink_python.cc python_bindings.cc)

gr_pybind_make_oot(tddModulePkg ../../.. gr::tddModulePkg "${tddModulePkg_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
    TARGET tddModulePkg_python
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tddModulePkg_python>
            ${PROJECT_BINARY_DIR}/test_modules/gnuradio/tddModulePkg/)

install(
    TARGETS tddModulePkg_python
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/tddModulePkg
    COMPONENT pythonapi)
