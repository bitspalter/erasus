//////////////////////////////////////////////////////////////////////////////////
// [ APP_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_App.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [ Konstructor ]  
//////////////////////////////////////////////////////////////////////////////////
C_App::C_App(){
  
   this->set_title("ERASUS");
   this->resize(550, 73);

   modus = C_APP_ERROR;
   
   ////////////////
   // Combo
   scombo.pass.set_size_request(220, 30);
   scombo.pass.append("British HMG IS5 (1 pass)");
   scombo.pass.append("US Airforce 5020 (3 passes)");
   scombo.pass.append("German VSITR (7 passes)");
   scombo.pass.set_active(0);
   
   //////////////////////////////////////////////////////////////
   // Button
   sbutton.open_file.set_label("open file");
   sbutton.open_file.set_size_request(110, 30);
   sbutton.open_file.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_open_file));
   
   sbutton.open_folder.set_label("open folder");
   sbutton.open_folder.set_size_request(110, 30);
   sbutton.open_folder.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_open_folder));
   
   sbutton.erase.set_label("erase");
   sbutton.erase.set_size_request(110, 30);
   sbutton.erase.signal_clicked().connect(sigc::mem_fun(*this, &C_App::on_button_erase));
   
   sbutton.erase.set_sensitive(false);
   
   ////////////////
   // label
   Pango::FontDescription fdsc("Courier New 10"); 
   
   slabel.eraser.set_label("welcome");
   slabel.eraser.set_alignment(0.0f, 0.5f);
   slabel.eraser.set_size_request(550, 30);
   slabel.eraser.override_font(fdsc);

   ///////////////////////////////////////////////////////
   // layout
   m_layout.put(sbutton.open_file,   220,  0);
   m_layout.put(sbutton.open_folder, 330,  0);
   m_layout.put(sbutton.erase,       440,  0);
   m_layout.put(scombo.pass,           0,  0);
   m_layout.put(slabel.eraser,        10, 40);

   add(m_layout);
   
   ////////////////////////
   
   signal_configure_event().connect(sigc::mem_fun(*this, &C_App::on_configure_changed), false);
   
   ////////////////////////
   
   show_all_children();
}
//////////////////////////////////////////////////////////////////////////////////
// [ Destructor ]
//////////////////////////////////////////////////////////////////////////////////
C_App::~C_App(){
  
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_open_file ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_open_file(){

   Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);

   int result = dialog.run();

   switch(result){
      case(Gtk::RESPONSE_OK):{
         sFile = dialog.get_filename();
         modus = C_APP_FILE;
         sbutton.erase.set_sensitive(true);
         slabel.eraser.set_label(sFile.data());
         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_open_folder ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_open_folder(){

   Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
   dialog.set_transient_for(*this);

   dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
   dialog.add_button("_Open",   Gtk::RESPONSE_OK);
   
   int result = dialog.run();
   
   switch(result){
      case(Gtk::RESPONSE_OK):{
         sFolder = dialog.get_filename();
         modus = C_APP_FOLDER;
         sbutton.erase.set_sensitive(true);
         slabel.eraser.set_label(sFolder.data());
         break;
      }
      case(Gtk::RESPONSE_CANCEL): break;
      default: break;
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_button_erase ]
//////////////////////////////////////////////////////////////////////////////////
void C_App::on_button_erase(){
   switch(modus){
      case C_APP_FILE:{
         if(erase_file(scombo.pass.get_active_row_number()) == C_APP_READY){
            slabel.eraser.set_label("all done");
            sbutton.erase.set_sensitive(false);
         }
         break;
      }
      case C_APP_FOLDER:{
         if(erase_folder(scombo.pass.get_active_row_number()) == C_APP_READY){
            slabel.eraser.set_label("all done");
            sbutton.erase.set_sensitive(false);
         }
         break;
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////
// [ erase_file ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::erase_file(int pass){

   int cBuffer = 4096;
   char pBuffer[cBuffer];
   
   string sError;
   
   fstream file;
   
   file.open(sFile, ios_base::in | ios_base::out | ios_base::binary);
   
   if(!file.is_open()){
      sError = "Error open file:";
      sError.append(sFile);
      slabel.eraser.set_label(sError.data());
      return(C_APP_ERROR);
   }
   
   ////////////////////////////////////
   
   file.seekg(0, file.end);
   int cdata = file.tellg();
   file.seekg(0, file.beg);
   
   ////////////////////////

   int cFile = cdata;
   int mFile = cFile - (cFile % cBuffer);

   ////////////////////////////////////
   
   const int* pValue = 0;
   int  cPass  = 0;
   
   switch(pass){
      default:
      case C_APP_PASS_1:{
         pValue = S_APP_PASS_1;
         cPass = sizeof(S_APP_PASS_1) / sizeof(int);
         break;   
      }
      case C_APP_PASS_3:{
         pValue = S_APP_PASS_3;
         cPass = sizeof(S_APP_PASS_3) / sizeof(int);
         break; 
      }
      case C_APP_PASS_7:{
         pValue = S_APP_PASS_7;
         cPass = sizeof(S_APP_PASS_7) / sizeof(int);
         break; 
      }
   }
   
   ////////////////////////////////////
   
   for(int nPass = 0; nPass < cPass; nPass++){
       
      file.seekg(0, file.beg);
      
      memset(pBuffer, pValue[nPass], cBuffer);
   
      ////////////////////////////////////
   
      for(int n = 0; n < mFile / cBuffer; n++){
         if(!file.write(pBuffer, cBuffer)){
            sError = "Error write file:";
            sError.append(sFile);
            slabel.eraser.set_label(sError.data());
            return(C_APP_ERROR); 
         }
      }
      
      if(!file.write(pBuffer, cFile % cBuffer)){
         sError = "Error write file:";
         sError.append(sFile);
         slabel.eraser.set_label(sError.data());
         return(C_APP_ERROR);  
      }
   }
   
   ////////////////////////////////////
   
   file.close();
   
   ///////////////
   
   if(CFileSystem.delfile(sFile.data()) != C_FILE_SYSTEM_READY){
      sError = "Error delete file:";
      sError.append(sFile);
      slabel.eraser.set_label(sError.data());
      return(C_APP_ERROR);   
   }
   
   return(C_APP_READY);  
}
//////////////////////////////////////////////////////////////////////////////////
// [ erase_folder ]
//////////////////////////////////////////////////////////////////////////////////
int C_App::erase_folder(int pass){

   C_DArray<char> CDA_Folder, CDA_Files;
   
   string sError;
   
   if(CFileSystem.rdir(sFolder.data(), &CDA_Folder, &CDA_Files) != C_FILE_SYSTEM_READY){
      sError = "Error list folder:";
      sError.append(sFolder);
      slabel.eraser.set_label(sError.data());
      return(C_APP_ERROR);  
   }
   
   for(int nFiles = 0; nFiles < CDA_Files.getnItems(); nFiles++){
      sFile = CDA_Files.getpItempData(nFiles)->getpBuffer();
      if(erase_file(pass) != C_APP_READY) return(C_APP_ERROR);
   }
   
   ///////////////
   
   if(CFileSystem.rdeldir(sFolder.data()) != C_FILE_SYSTEM_READY){
      sError = "Error delete folder:";
      sError.append(sFolder);
      slabel.eraser.set_label(sError.data());
      return(C_APP_ERROR);     
   }
   
   return(C_APP_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [ on_configure_changed ]
//////////////////////////////////////////////////////////////////////////////////
bool C_App::on_configure_changed(GdkEventConfigure* configure_event){
   slabel.eraser.set_size_request(configure_event->width - 20, 30);
   return(false);
}
