#include <cstdlib>

#include <gtkmm-3.0/gtkmm.h>

#include <Importer.hpp>

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    Gtk::Window window;
    window.set_default_size(300, 400);
    window.set_resizable(false);
    window.set_title("MusicList");

    return app->run(window);
}
