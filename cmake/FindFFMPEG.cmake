find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

set(FFMPEG_FOUND 1)

# Include directories
include_directories(${FFMPEG_INCLUDE_DIRS})