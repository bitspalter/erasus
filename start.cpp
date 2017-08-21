#include "C_App.hpp"
#include <gtkmm/application.h>

int main(int argc, char *argv[]){
  
   auto app = Gtk::Application::create(argc, argv, "org.gtkmm.eraser");

   C_App CApp;

   //Shows the window and returns when it is closed.
   return app->run(CApp);
}
