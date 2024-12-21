# Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

include(CMakeFindDependencyMacro)

# Include our export. This imports all of our targets.
include("${CMAKE_CURRENT_LIST_DIR}/CppUtilsExport.cmake")

#
# Add alias targets whose names match those from the project file.
#
# It's important to make sure the names are the same for consistency. Namely, so that build
# interface users can use `OVERRIDE_FIND_PACKAGE` with `FetchContent_Declare()`.
#

add_library(::CppUtils ALIAS CppUtils)
