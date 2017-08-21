//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// ERASUS
//
// [::Last modi: 20.08.17 L.ey (µ~)::]
//
//
#ifndef _C_APP_H_
 #define _C_APP_H_

 #include <iostream>
 #include <fstream>
 using namespace std;
 
 #include <gtkmm.h>
 #include <gtkmm/window.h>
 #include <gtkmm/button.h>
 #include <gtkmm/comboboxtext.h>
 #include <gtkmm/layout.h>
 #include <gtkmm/filechooser.h>
 
 #include "C_File_System.hpp"
 
//////////////////////////////////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////////////////////////////////
 
 const int C_APP_ERROR = 0x00;
 const int C_APP_READY = 0x01;
   
 const int C_APP_FILE   = 0x01;
 const int C_APP_FOLDER = 0x02;
 
 const int C_APP_PASS_1 = 0x00;
 const int C_APP_PASS_3 = 0x01;
 const int C_APP_PASS_7 = 0x02;
 
 const int S_APP_PASS_1[] = {0};
 const int S_APP_PASS_3[] = {0, 255, 85};
 const int S_APP_PASS_7[] = {0, 255, 0, 255, 0, 255, 170};
 
//////////////////////////////////////////////////////////////////////////////////
// STRUCT
//////////////////////////////////////////////////////////////////////////////////

 struct SButton {
    Gtk::Button open_file;
    Gtk::Button open_folder;
    Gtk::Button erase;
 };
 
 //////////////////////////////
 
 struct SLabel {
    Gtk::Label eraser;
 };
 
 //////////////////////////////
 
 struct SCombo {
    Gtk::ComboBoxText pass;
 };
 
//////////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////////
 
 class C_App : public Gtk::Window {
  
    public:
      
       C_App();
       virtual ~C_App();

    protected:

       Gtk::Layout m_layout;

       SButton sbutton;
       SLabel  slabel;
       SCombo  scombo;
       
       ////////////////////////
       
       string sFile, sFolder;
       
       int modus;
       
       C_File_System CFileSystem;
       
       ////////////////////////
       
       int erase_file(int pass);
       int erase_folder(int pass);
       
       ////////////////////////
       
       //Signal handlers:
       void on_button_open_file();
       void on_button_open_folder();
       void on_button_erase();
       bool on_configure_changed(GdkEventConfigure* configure_event);
 };

#endif // _C_APP_H_
