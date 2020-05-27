gtest = dependency('GTest', required: true)

track_test = executable('track-test', ['TrackTest.cpp'],
    include_directories: [proj_include],
    dependencies: [gtest],
    link_with: [lib_music_data])

test('Track Test', track_test)

album_test = executable('album-test', ['AlbumTest.cpp'],
    include_directories: [proj_include],
    dependencies: [gtest],
    link_with: [lib_music_data])

test('Album Test', album_test)

# pic_test = executable('pic-test', ['BlockPictureTest.cpp'],
#     include_directories: [proj_include],
#     dependencies: [gtest],
#     link_with: [lib_music_data]
# )

# test('Block Picture Test', pic_test)
