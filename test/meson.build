gtest = dependency('GTest', required: true)

track_test = executable('track-test', ['TrackTest.cpp'],
    include_directories: [proj_include],
    dependencies: [gtest],
    link_with: [lib_track])

test('Track Test', track_test)