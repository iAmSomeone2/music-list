gtkmm = dependency('gtkmm-3.0', required: true, version: '>=3.20.0')
glibmm = dependency('glibmm-2.4', required: true)

gui_exe = executable('musiclist-gtk', ['music_list_gtk.cpp'],
    include_directories: [core_include],
    dependencies: [jsoncpp, gtkmm, glibmm],
    link_with: [lib_music_data]
)