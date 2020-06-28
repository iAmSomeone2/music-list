add_executable(albumtest "AlbumTest.cpp")
target_link_libraries(albumtest GTest::GTest musicdata)
add_test(album-test albumtest)

add_executable(tracktest "TrackTest.cpp")
target_link_libraries(tracktest GTest::GTest musicdata)
add_test(track-test tracktest)