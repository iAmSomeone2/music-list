add_executable(musiclist "music_list.cpp")
target_link_libraries(musiclist musicdata ${JsonCpp_LIBRARIES})
