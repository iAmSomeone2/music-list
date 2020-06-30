# Music Data Library

list(APPEND MUSIC_DATA_SRCS
    "Track.cpp" "Track.hpp"
    "Album.cpp" "Album.hpp"
    "Importer.cpp" "Importer.hpp"
)

find_package(FLAC REQUIRED)
find_package(Opus REQUIRED)

add_library(musicdata STATIC ${MUSIC_DATA_SRCS} ../config.hpp.in)

target_include_directories(musicdata PRIVATE ${OPUS_INCLUDE_DIR} ${FLAC_INCLUDE_DIRS})

target_link_libraries(musicdata
    ${FLAC_LIBRARY}
    ${OPUS_LIBRARY}
    ${OPUSFILE_LIBRARY}
    ${JsonCpp_LIBRARIES}
)