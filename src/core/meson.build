# Music Data Library

music_data_srcs = [
        'Track.cpp', 'Track.hpp',
        'Album.cpp', 'Album.hpp', 
        'Importer.cpp', 'Importer.hpp'
]

flac = dependency('flac', required: true)
opus = dependency('opus', required: true)
opusfile = dependency('opusfile', required: true)

lib_music_data = static_library('musicdata', music_data_srcs,
        dependencies: [flac, opus, opusfile, jsoncpp]
)
