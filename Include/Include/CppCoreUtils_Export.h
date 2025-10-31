// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef CPPCOREUTILS_EXPORT_H
#define CPPCOREUTILS_EXPORT_H

// Include the appropriate "export header" file depending on which type of this library is being compiled.

#if __has_include(<cppcoreutils_shared_export.h>)
#   include <cppcoreutils_shared_export.h>
#elif __has_include(<cppcoreutils_module_export.h>)
#   include <cppcoreutils_module_export.h>
#elif __has_include(<cppcoreutils_object_export.h>)
#   include <cppcoreutils_object_export.h>
#elif __has_include(<cppcoreutils_static_export.h>)
#   include <cppcoreutils_static_export.h>
#else
    // Since no generated header files exist, it must mean we are an interface library (being used as a header-only library).
#   include <cppcoreutils_include_export.h>
#endif

// Undef the generated macros that we've decided not to use.

#ifdef CPPCOREUTILS_DEPRECATED
#   undef CPPCOREUTILS_DEPRECATED
#endif

#ifdef CPPCOREUTILS_DEPRECATED_EXPORT
#   undef CPPCOREUTILS_DEPRECATED_EXPORT
#endif

#ifdef CPPCOREUTILS_DEPRECATED_NO_EXPORT
#   undef CPPCOREUTILS_DEPRECATED_NO_EXPORT
#endif

#ifdef CPPCOREUTILS_NO_DEPRECATED
#   undef CPPCOREUTILS_NO_DEPRECATED
#endif

#endif // #ifndef CPPCOREUTILS_EXPORT_H
