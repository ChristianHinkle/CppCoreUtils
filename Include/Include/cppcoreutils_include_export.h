// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef CPPCOREUTILS_INCLUDE_EXPORT_H
#define CPPCOREUTILS_INCLUDE_EXPORT_H

// Since interface libraries cannot generate header files during their cmake configuration, we author our
// own "export header" file here, which defines the export macros as a noop.

#define CPPCOREUTILS_EXPORT
#define CPPCOREUTILS_NO_EXPORT

#endif // #ifndef CPPCOREUTILS_INCLUDE_EXPORT_H
