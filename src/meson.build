# Music Data Library

music_data_srcs = ['Track.cpp', 'Track.hpp', 'Album.cpp', 'Album.hpp', 'BlockPicture.cpp', 'BlockPicture.hpp']

flac = dependency('flac', required: true)
vorbis = dependency('vorbis', required: true)

lib_music_data = static_library('musicdata', music_data_srcs,
        dependencies: [flac, vorbis, jsoncpp]
)

# Main executable

music_list = executable('musiclist', ['music_list.cpp'],
        dependencies: [jsoncpp],
        include_directories: [proj_include],
        link_with: [lib_music_data]
)
