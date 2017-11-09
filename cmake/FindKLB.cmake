# - Find KLB
#
# Find the KLB includes and libraries
# This module defines
#  KLB_FOUND           - TRUE if found, FALSE otherwise
#  KLB_INCLUDE_DIR     - Include directories for KLB
#  KLB_LIBRARIES       - The libraries to link against to use KLB

FIND_PACKAGE(PackageHandleStandardArgs)

SET(KLB_SEARCHPATH
    /usr/include/
    /usr/local/include/
    /opt/include/
    /opt/local/include/
    )

FIND_PATH(KLB_INCLUDE_DIR
    NAMES klb_imageIO.h
    PATHS ${KLB_SEARCHPATH}
    DOC "The KLB include directory")

FIND_LIBRARY(KLB_LIBRARY
    NAMES klb
    PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
    /opt/lib
    /opt/local/lib
    /opt/lib64
    /opt/local/lib64
    DOC "The KLB library")

SET(KLB_LIBRARIES ${KLB_LIBRARY})

IF(KLB_INCLUDE_DIR AND KLB_LIBRARIES)
    SET(KLB_FOUND TRUE)
ELSE(KLB_INCLUDE_DIR AND KLB_LIBRARIES)
    SET(KLB_FOUND FALSE)
ENDIF(KLB_INCLUDE_DIR AND KLB_LIBRARIES)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KLB REQUIRED_VARS KLB_INCLUDE_DIR KLB_LIBRARIES)

MARK_AS_ADVANCED( KLB_INCLUDE_DIR KLB_LIBRARIES )
