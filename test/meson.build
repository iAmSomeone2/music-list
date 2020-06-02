if get_option('enable_tests')
    gtest = dependency('GTest', required: false)
else
    gtest = disabler()
endif


if gtest.found()
    track_test = executable('track-test', ['TrackTest.cpp'],
        include_directories: [core_include],
        dependencies: [gtest, jsoncpp],
        link_with: [lib_music_data])

    test('Track Test', track_test)

    album_test = executable('album-test', ['AlbumTest.cpp'],
        include_directories: [core_include],
        dependencies: [gtest, jsoncpp],
        link_with: [lib_music_data])

    test('Album Test', album_test)

    importer_test = executable('importer-test', ['ImporterTest.cpp'],
        include_directories: [core_include],
        dependencies: [gtest, jsoncpp],
        link_with: [lib_music_data]
    )
    test('Importer Test', importer_test, timeout: 120)
endif
