# - Find NIfTI
#
# Find the NIfTI includes and libraries
# This module defines
#  NIfTI_FOUND           - TRUE if found, FALSE otherwise
#  NIfTI_INCLUDE_DIR     - Include directories for NIfTI
#  NIfTI_LIBRARIES       - The libraries to link against to use NIfTI

FIND_PACKAGE(PackageHandleStandardArgs)

SET(NIfTI_SEARCHPATH
    /usr/include/nifti
    /usr/local/include/nifti
    /opt/include/nifti
    /opt/local/include/nifti
    )

FIND_PATH(NIfTI_INCLUDE_DIR
    NAMES nifti1_io.h
    PATHS ${NIfTI_SEARCHPATH}
    DOC "The NIfTI include directory")

FIND_LIBRARY(NIfTI_LIBRARY
    NAMES niftiio libniftiio
    PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
    /opt/lib
    /opt/local/lib
    /opt/lib64
    /opt/local/lib64
    DOC "The NIfTI library")

FIND_LIBRARY(Z_LIBRARY
    NAMES znz libznz
    PATHS
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
    /opt/lib
    /opt/local/lib
    /opt/lib64
    /opt/local/lib64
    DOC "The Z library")

SET(NIfTI_LIBRARIES ${NIfTI_LIBRARY} ${Z_LIBRARY})

IF(NIfTI_INCLUDE_DIR AND NIfTI_LIBRARIES)
    SET(NIfTI_FOUND TRUE)
ELSE(NIfTI_INCLUDE_DIR AND NIfTI_LIBRARIES)
    SET(NIfTI_FOUND FALSE)
ENDIF(NIfTI_INCLUDE_DIR AND NIfTI_LIBRARIES)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NIfTI REQUIRED_VARS NIfTI_INCLUDE_DIR NIfTI_LIBRARIES)

MARK_AS_ADVANCED( NIfTI_INCLUDE_DIR NIfTI_LIBRARIES )
