file(COPY db DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

add_subdirectory(core)
add_subdirectory(cli)

# if (ENABLE_GUI)
#     add_subdirectory(gui)
# endif()