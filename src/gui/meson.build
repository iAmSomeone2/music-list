gtkmm = dependency('gtkmm-3.0', required: true, version: '>=3.20.0')
glibmm = dependency('glibmm-2.4', required: true)

ui_srcs = ['music_list_gtk.cpp',
    'StartupWindow.cpp', 'StartupWindow.hpp'
]

gui_exe = executable('musiclist-gtk', ui_srcs,
    include_directories: [core_include],
    dependencies: [jsoncpp, gtkmm, glibmm],
    link_with: [lib_music_data]
)