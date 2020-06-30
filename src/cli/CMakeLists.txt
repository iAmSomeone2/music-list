add_executable(musiclist "music_list.cpp" ../config.hpp.in)
target_link_libraries(musiclist musicdata ${JsonCpp_LIBRARIES})
