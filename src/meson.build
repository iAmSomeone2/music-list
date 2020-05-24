track_srcs = ['Track.cpp', 'Track.hpp']

flac = dependency('flac', required: true)
vorbis = dependency('vorbis', required: true)

lib_track = static_library('track', track_srcs,
        dependencies: [flac, vorbis, jsoncpp])

album_srcs = ['Album.cpp', 'Album.hpp']

lib_album = static_library('album', album_srcs,
        dependencies: [jsoncpp],
        link_with: [lib_track])
