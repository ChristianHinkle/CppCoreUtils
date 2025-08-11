# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 3.24)

project(CppUtils_Core
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "https://github.com/ChristianHinko/CppUtils_Core"
  LANGUAGES CXX
  )

#
# Add targets.
#
# We namespace target names using underscore characters "_". Not namespacing your target names can result in their
# file names not be specific enough. E.g., for the DAM library of MeddySDK, we want its library file to be named
# something like "MeddySDK_DAM.lib" instead of just "DAM.lib" - because that would be super vague.
#

add_library(CppUtils_Core)

#
# Add alias targets.
#
# We name these with special characters that filenames can't have. We use these aliases instead of the direct
# target names where possible so that they can't be be interpreted as filenames.
#
# It's conventional to use "::" in these names, like namespaces. If a target doesn't have anything to namespace in its
# name then just prepend "::" to it, analogous to how we indicate global scope in cpp.
#
# The aliases set up here get used by build interface users. Make sure the same aliases are also set up for install
# interface users somewhere else. We do that in the export's config file. It's important to make sure the names are
# the same in both cases for consistency. Namely, so that build interface users can use `OVERRIDE_FIND_PACKAGE` with
# `FetchContent_Declare()`.
#

add_library(CppUtils::Core ALIAS CppUtils_Core)

# List out required language features.
target_compile_features(CppUtils_Core
  PUBLIC
    cxx_std_20
  )

# Generate cpp header file with export macros for our target.
include(GenerateExportHeader)
generate_export_header(CppUtils_Core
  # Set the export macro name to be in the "_API" format which is most-conventional. This
  # is important for compatibility with other build systems that may have this convention
  # hard-coded. E.g., Unreal Engine's build system.
  EXPORT_MACRO_NAME CPPUTILS_CORE_API
  )

# Precompile any CMake-specific headers so our source files can ommit those includes and
# be built independently from CMake.
target_precompile_headers(CppUtils_Core
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/cpputils_core_export.h"
  )

#
# Add all header files and set up their include directories.
#
# The `BASE_DIRS` of file sets automatically get added to the target's include
# directories. So we don't use the `target_include_directories()` command anymore
# because that would be redundant.
#
# The file set of public headers is important because we use it to install
# them for install interface users. It's very important to list out all public headers
# in this file set so that install interface users have them.
#
# A nice benefit of listing all header files is that they can be easily-discoverable by IDEs.
#
target_sources(CppUtils_Core
  PUBLIC
    FILE_SET cpputils_core_public_headers
      TYPE HEADERS
      BASE_DIRS
        # Base directory of public headers.
        "${CppUtils_Core_SOURCE_DIR}/Source/Public"
        # Base directory of generated headers.
        "${CMAKE_CURRENT_BINARY_DIR}"
      FILES
        # Add all public header files.
        "${CMAKE_CURRENT_BINARY_DIR}/cpputils_core_export.h"
        "${CppUtils_Core_SOURCE_DIR}/Source/Public/CppUtils_Core.h"
        "${CppUtils_Core_SOURCE_DIR}/Source/Public/CppUtils_Core_Concepts.h"
  PRIVATE
    FILE_SET cpputils_core_private_headers
      TYPE HEADERS
      BASE_DIRS
        #
        # Base directory of private headers.
        #
        # It's nice to leave the "Private" directory out of this base include path so that, for
        # cpp include statements, you have to type "Private/" to access the non-public headers. This
        # makes it obvious for programmers when a header shouldn't be included in public files.
        #
        "${CppUtils_Core_SOURCE_DIR}/Source"
      FILES
        # Add all private header files.
  )

# Add all source files.
target_sources(CppUtils_Core
  PRIVATE
    "${CppUtils_Core_SOURCE_DIR}/Source/Private/CppUtils_Core.cpp"
    "${CppUtils_Core_SOURCE_DIR}/Source/Private/CppUtils_Core_Concepts.cpp"
  )

include(GNUInstallDirs)

# Associate our target with an export and set up installation.
install(TARGETS CppUtils_Core
  EXPORT CppUtils_CoreExport
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  #
  # Add to interface include directories.
  #
  # Note: We take care of install-specific include directories here instead of `target_include_directories()`.
  #
  # Note: It's important to make sure cpp include statements of public headers are compatible between
  # install interface users and build interface users. E.g., a user's include statements should work for
  # both packaged library usage and subproject library usage, without having to adapt their code to either use case.
  #
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUtils_Core"
  # Copy over public headers.
  FILE_SET cpputils_core_public_headers DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/CppUtils_Core"
  )

# Generate script that imports targets associated with our export.
install(EXPORT CppUtils_CoreExport
  FILE "CppUtils_CoreExport.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CppUtils_Core"
  )

# Create a package version file for our export.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/CppUtils_CoreConfigVersion.cmake"
  VERSION ${CppUtils_Core_VERSION}
  COMPATIBILITY SameMajorVersion
  )

# Copy any files for downstream usage to the install directory.
install(
  FILES
    "${CppUtils_Core_SOURCE_DIR}/CppUtils_CoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CppUtils_CoreConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CppUtils_Core"
  )

# Set some simple values for CPack based on this current project. The default
# values for these are specific to the top-level project but we want to allow for
# having several projects generate their own CPack configs without conflicting.
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PROJECT_NAME};ALL;/")

# Output the generated CPack configs in a directory specific to this project. This is
# nice so we can generate our own config without conflicting with other projects.
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/CPackConfig.cmake")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/CPackSourceConfig.cmake")

# Generate a CPackConfig.cmake file for this project.
include(CPack)
