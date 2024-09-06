#pragma once

#define XSTR(v) STR(v)
#define STR(v) #v

// #define RELEASE 0
// #define INDEV 1
// #define RELEASE_CANDIDATE 2
// #define BETA 3
// #define NIGHTY 4


// (8 bit major, 8 bit minor, 8 bit patch), 2 bit release type, 6 bit release var 
// (note that both release and release candidate are considered as 0 for type, 
// but for release, the release var is always 0, and for release candidate, the release var is the release candidate number)
#define MATRIXOS_VERSION_ID_TEMPLATE (((MATRIXOS_MAJOR_VER & 0xFF) << 24) | ((MATRIXOS_MINOR_VER & 0x7F) << 16) | ((MATRIXOS_PATCH_VER & 0xFF) << 8))



// 5 bit major, 6 bit minor, 5 bit patch
#define MATRIXOS_VERSION_ID_16 (((MATRIXOS_MAJOR_VER & 0x1F) << 11) | ((MATRIXOS_MINOR_VER & 0x3F) << 5) | ((MATRIXOS_PATCH_VER & 0x1F)))

#if (MATRIXOS_PATCH_VER > 0)
#define MATRIXOS_MINOR_VERSION_STRING "." XSTR(MATRIXOS_PATCH_VER)
#else
#define MATRIXOS_MINOR_VERSION_STRING // So it will be x.y instead of x.y.0
#endif

#if defined(MATRIXOS_BUILD_RELEASE)
#define MATRIXOS_BUILD_VER 0
#define MATRIXOS_LOG_LEVEL LOG_LEVEL_INFO
#define MATRIXOS_VERSION_STRING XSTR(MATRIXOS_MAJOR_VER) "." XSTR(MATRIXOS_MINOR_VER) MATRIXOS_MINOR_VERSION_STRING
#define MATRIXOS_VERSION_ID (MATRIXOS_VERSION_ID_TEMPLATE)
#elif defined(MATRIXOS_BUILD_RELEASE_CANDIDATE)
#define MATRIXOS_BUILD_VER 2
#define MATRIXOS_LOG_LEVEL LOG_LEVEL_INFO
#define MATRIXOS_VERSION_STRING XSTR(MATRIXOS_MAJOR_VER) "." XSTR(MATRIXOS_MINOR_VER) MATRIXOS_MINOR_VERSION_STRING " RC" XSTR(MATRIXOS_PATCH_VER)
#define MATRIXOS_VERSION_ID (MATRIXOS_VERSION_ID_TEMPLATE | (MATRIXOS_RELEASE_VER & 0x3F))
#elif defined(MATRIXOS_BUILD_BETA)
#define MATRIXOS_BUILD_VER 3
#define MATRIXOS_LOG_LEVEL LOG_LEVEL_INFO
#define MATRIXOS_VERSION_STRING XSTR(MATRIXOS_MAJOR_VER) "." XSTR(MATRIXOS_MINOR_VER) MATRIXOS_MINOR_VERSION_STRING " Beta " XSTR(MATRIXOS_PATCH_VER)
#define MATRIXOS_VERSION_ID (MATRIXOS_VERSION_ID_TEMPLATE | (1 << 6) | (MATRIXOS_RELEASE_VER & 0x3F))
#elif defined(MATRIXOS_BUILD_NIGHTY)
#define MATRIXOS_BUILD_VER 4
#define MATRIXOS_LOG_LEVEL LOG_LEVEL_INFO
#define MATRIXOS_VERSION_STRING XSTR(MATRIXOS_MAJOR_VER) "." XSTR(MATRIXOS_MINOR_VER) MATRIXOS_MINOR_VERSION_STRING XSTR(MATRIXOS_PATCH_VER) " Nighty " __DATE__
#define MATRIXOS_VERSION_ID (MATRIXOS_VERSION_ID_TEMPLATE | (2 << 6) | (MATRIXOS_RELEASE_VER & 0x3F))
#elif defined(MATRIXOS_BUILD_INDEV)
#define MATRIXOS_BUILD_VER 1
#define MATRIXOS_LOG_LEVEL LOG_LEVEL_VERBOSE
#define MATRIXOS_VERSION_STRING XSTR(MATRIXOS_MAJOR_VER) "." XSTR(MATRIXOS_MINOR_VER) MATRIXOS_MINOR_VERSION_STRING " InDev " __DATE__ " " __TIME__
#define MATRIXOS_VERSION_ID (MATRIXOS_VERSION_ID_TEMPLATE | (3 << 6) | (MATRIXOS_RELEASE_VER & 0x3F))
#else
#error "No Release Version Specified"
#endif

