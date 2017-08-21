//////////////////////////////////////////////////////////////////////////////////
// [ File_System_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 20.08.17 L.ey (�~)::]  
//
// 
// [::alpha 0.2]
//
#ifndef _C_FILE_SYSTEM_H_
 #define _C_FILE_SYSTEM_H_
 
 #include <iostream>
 #include <fstream>
  
 using namespace std;

 #include <sys/stat.h>
 #include <sys/statvfs.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <dirent.h>

 #include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////

 static const int C_FILE_SYSTEM_READY  = 0x01;
 static const int C_FILE_SYSTEM_ERROR  = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 class C_File_System {
     
    public:

       C_File_System(){};  
      ~C_File_System(){};  

       //////////////////////////////////

       int dir(const char* psPath, C_DArray<char>* pDA_Folder, C_DArray<char>* pDA_File);
       int makedir(const char* psPath, mode_t mode);
       int deldir(const char* psPath);
       
       int copyfile(const char* psSource, const char* psDest);
       int delfile(const char* psPath);
       int movefile(const char* psSource, const char* psDest);
       
       //////////////////////////////////
       // (r) == recursion
       
       int rdir(const char* psPath, C_DArray<char>* pDA_Folder, C_DArray<char>* pDA_File);
       int rcopydir(const char* psSource, const char* psDest);
       int rmovedir(const char* psSource, const char* psDest);
       int rdeldir(const char* psSource);
       
       int getrdirsize(const char* psPath, unsigned long* pSize);

    protected:

       int rmakedir(C_DArray<char>* pDAFolder);
       int rcopyfile(C_DArray<char>* pDA_Source, C_DArray<char>* pDA_Dest);
       int rdelfile(C_DArray<char>* pDA_File);
       int rdeletedir(C_DArray<char>* pDA_Folder);
       
       int getfreespace(const char* psPath, unsigned long* pFree);
 };

#endif // _C_FILE_SYSTEM_H_
