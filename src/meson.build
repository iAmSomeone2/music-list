track_srcs = ['Track.cpp', 'Track.hpp']

flac = dependency('flac', required: true)
vorbis = dependency('vorbis', required: true)
opus = dependency('opus', required: true)

lib_track = static_library('track', track_srcs,
        dependencies: [flac, vorbis, opus])

album_srcs = ['Album.cpp', 'Album.hpp']

lib_album = static_library('album', album_srcs,
        link_with: [lib_track])
