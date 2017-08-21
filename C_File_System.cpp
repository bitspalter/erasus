//////////////////////////////////////////////////////////////////////////////////
// [ File_System_Class_Source ]
//////////////////////////////////////////////////////////////////////////////////

#include "C_File_System.hpp"

//////////////////////////////////////////////////////////////////////////////////
// [dir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::dir(const char* psPath, C_DArray<char>* pDA_Folder, C_DArray<char>* pDA_File){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;
   C_Array<char>* pCA_Folder = 0;
   C_Array<char>* pCA_File   = 0;
   
   if(!psPath || !pDA_Folder || !pDA_File) return(C_FILE_SYSTEM_ERROR);

   dp = opendir(psPath);
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

               if(!(pCA_File = pDA_File->addItem(strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_File->getpBuffer(), de->d_name, strlen(de->d_name) + 1);
               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

               if(!(pCA_Folder = pDA_Folder->addItem(strlen(de->d_name) + 1))) return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_Folder->getpBuffer(), de->d_name, strlen(de->d_name) + 1);
               break;

            default:      
               break;
         }
      }
      closedir(dp);
   }

   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [makedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::makedir(const char* psPath, mode_t mode){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(mkdir(psPath, mode) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [deldir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::deldir(const char* psPath){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(rmdir(psPath) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [copyfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::copyfile(const char* psSource, const char* psDest){
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
   ifstream src(psSource, ios::binary);
   ofstream dst(psDest,   ios::binary);
   dst << src.rdbuf();
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [delfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::delfile(const char* psPath){
   if(!psPath) return(C_FILE_SYSTEM_ERROR);
   if(remove(psPath) != 0) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [movefile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::movefile(const char* psSource, const char* psDest){
   if(this->copyfile(psSource, psDest) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   if(this->delfile(psSource)          != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [Rekursiv Function]
//////////////////////////////////////////////////////////////////////////////////
// [rdir] return DArray with (char)Path\FolderName and 
//               DArray with (char)Path\FileName
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdir(const char* psPath, C_DArray<char>* pDA_Folder, C_DArray<char>* pDA_File){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;
   C_Array<char>* pCA_Folder = 0;
   C_Array<char>* pCA_File   = 0;
   
   string spath;
   
   if(!psPath || !pDA_Folder || !pDA_File) return(C_FILE_SYSTEM_ERROR);
   
   spath.append(psPath);

   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   //////////////////////////////////////////////////////////////
   
   dp = opendir(spath.c_str());
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

               if(!(pCA_File = pDA_File->addItem(spath.size() + strlen(de->d_name) + 1))) 
                  return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_File->getpBuffer(), spath.c_str(), spath.size());
               memcpy(pCA_File->getpBuffer() + spath.size(), de->d_name, strlen(de->d_name) + 1);
               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

               if(!(pCA_Folder = pDA_Folder->addItem(spath.size() + strlen(de->d_name) + 1))) 
                  return(C_FILE_SYSTEM_ERROR);
               memcpy(pCA_Folder->getpBuffer(), spath.c_str(), spath.size());
               memcpy(pCA_Folder->getpBuffer() + spath.size(), de->d_name, strlen(de->d_name) + 1);
               this->rdir(pCA_Folder->getpBuffer(), pDA_Folder, pDA_File); // rekursion
               break;
               
            default: break;
         }
      }
      closedir(dp);
   }
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rcopydir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rcopydir(const char* psSource, const char* psDest){
  
   C_DArray<char> DA_Folder;
   C_DArray<char> DA_File;
   C_DArray<char> DA_MkDir;
   C_DArray<char> DA_CpFile;

   C_Array<char>* pCA_Folder = 0;
   C_Array<char>* pCA_MkDir  = 0;

   C_Array<char>* pCA_Files  = 0;
   C_Array<char>* pCA_CPFile = 0;
   
   char* psResult = 0;
   char* psMkDir  = 0;
   char* psFileD  = 0;

   string spath, dpath;
   
   ///////////////////////////////////////////////////////////////////////
   
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
  
   spath.append(psSource);
   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   dpath.append(psDest);
   if(dpath[dpath.size() - 1] != '/') dpath.append("/");
   
   ///////////////////////////////////////////////////////////////////////
   
   if(this->rdir(psSource, &DA_Folder, &DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
  
   pCA_MkDir = DA_MkDir.addItem(dpath.size() + 1);
   psMkDir   = pCA_MkDir->getpBuffer();
   
   memcpy(psMkDir, dpath.c_str(), dpath.size());
   
   for(unsigned long nFolder = 0; nFolder < DA_Folder.getnItems(); nFolder++){
      psResult  = DA_Folder.getpItempData(nFolder)->getpBuffer();
      pCA_MkDir = DA_MkDir.addItem(dpath.size() + spath.size() - strlen(psResult) + 1);
      psMkDir   = pCA_MkDir->getpBuffer();
      memcpy(psMkDir, dpath.c_str(), dpath.size());
      memcpy(&psMkDir[dpath.size()], &psResult[spath.size()], strlen(&psResult[spath.size()]) + 1);
   }
   
   if(this->rmakedir(&DA_MkDir) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);

   ///////////////////////////////////////////////////////////////////////

   for(int nFiles = 0; nFiles < DA_File.getnItems(); nFiles++){
      psResult   = DA_File.getpItempData(nFiles)->getpBuffer();
      pCA_CPFile = DA_CpFile.addItem(dpath.size() + strlen(psResult) + 1);
      psFileD    = pCA_CPFile->getpBuffer();
      memcpy(psFileD, dpath.c_str(), dpath.size());
      memcpy(&psFileD[dpath.size()], &psResult[spath.size()], strlen(&psResult[spath.size()]) + 1);
   }

   if(this->rcopyfile(&DA_File, &DA_CpFile) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rmakedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rmakedir(C_DArray<char>* pDAFolder){

   if(!pDAFolder) return(C_FILE_SYSTEM_ERROR);
  
   for(int nFolder = 0; nFolder < pDAFolder->getnItems(); nFolder++){
      if(mkdir(pDAFolder->getpItempData(nFolder)->getpBuffer(), S_IRWXU) != 0) return(C_FILE_SYSTEM_ERROR);
   }
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rcopyfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rcopyfile(C_DArray<char>* pDA_S, C_DArray<char>* pDA_D){

   if(!pDA_S || !pDA_D) return(C_FILE_SYSTEM_ERROR);
   
   for(int nFiles = 0; nFiles < pDA_S->getnItems(); nFiles++){
      if(this->copyfile(pDA_S->getpItempData(nFiles)->getpBuffer(), 
                        pDA_D->getpItempData(nFiles)->getpBuffer()) != C_FILE_SYSTEM_READY) 
         return(C_FILE_SYSTEM_ERROR);
   }

   return(C_FILE_SYSTEM_READY); 
}
//////////////////////////////////////////////////////////////////////////////////
// [rmovedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rmovedir(const char* psSource, const char* psDest){
  
   if(!psSource || !psDest) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rcopydir(psSource, psDest) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdeldir(psSource) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rmovedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdeldir(const char* psSource){
  
   C_DArray<char> DA_Folder, DA_File;
   C_Array<char>* pCA_Folder = 0;
   
   if(!psSource) return(C_FILE_SYSTEM_ERROR);
   
   pCA_Folder = DA_Folder.addItem(strlen(psSource) + 1);
   if(!pCA_Folder) return(C_FILE_SYSTEM_ERROR);

   memcpy(pCA_Folder->getpBuffer(), psSource, strlen(psSource) + 1);
   
   if(this->rdir(psSource, &DA_Folder, &DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdelfile(&DA_File) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   if(this->rdeletedir(&DA_Folder) != C_FILE_SYSTEM_READY) return(C_FILE_SYSTEM_ERROR);
   
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rdelfile]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdelfile(C_DArray<char>* pDA_File){

   if(!pDA_File) return(C_FILE_SYSTEM_ERROR);
  
   for(int nFile = 0; nFile < pDA_File->getnItems(); nFile++){
      if(this->delfile(pDA_File->getpItempData(nFile)->getpBuffer()) != C_FILE_SYSTEM_READY) 
         return(C_FILE_SYSTEM_ERROR);
   }

   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [rdeletedir]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::rdeletedir(C_DArray<char>* pDA_Folder){

   if(!pDA_Folder) return(C_FILE_SYSTEM_ERROR);  
  
   int cFolder = pDA_Folder->getnItems();
  
   for(int nFolder = cFolder - 1; nFolder > 0; nFolder--)
      if(this->deldir(pDA_Folder->getpItempData(nFolder)->getpBuffer()) != C_FILE_SYSTEM_READY) 
         return(C_FILE_SYSTEM_ERROR);

   if(this->deldir(pDA_Folder->getpItempData(0)->getpBuffer()) != C_FILE_SYSTEM_READY) 
      return(C_FILE_SYSTEM_ERROR);
  
   return(C_FILE_SYSTEM_READY);
}
//////////////////////////////////////////////////////////////////////////////////
// [getrdirsize]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::getrdirsize(const char* psPath, unsigned long* pSize){
  
   const unsigned char bFile   = 0x08;
   const unsigned char bFolder = 0x04;
   DIR*    dp = 0;
   dirent* de = 0;

   string spath, dpath;
   
   if(!psPath || !pSize) return(C_FILE_SYSTEM_ERROR);
   
   spath.append(psPath);

   if(spath[spath.size() - 1] != '/') spath.append("/");
   
   //////////////////////////////////////////////////////////////
   
   dp = opendir(spath.c_str());
  
   if(dp){
      while(true){

         if((de = readdir(dp)) == NULL) break;

         switch(de->d_type){
            case bFile:

               dpath.clear();
               dpath.append(spath);
               dpath.append(de->d_name);

               struct stat st;
               stat(dpath.c_str(), &st);

               (*pSize) += st.st_size;

               break;

            case bFolder: 

               if((de->d_name[0x00] == '.' && de->d_name[0x01] == 0x00) ||
                  (de->d_name[0x00] == '.' && de->d_name[0x01] == '.' && de->d_name[0x02] == 0x00)) break;

               dpath.clear();
               dpath.append(spath);
               dpath.append(de->d_name);

               this->getrdirsize((char*)dpath.c_str(), pSize); // rekursion

               break;

            default: break;
         }
      }
      closedir(dp);
   }
   return(C_FILE_SYSTEM_READY);   
}
//////////////////////////////////////////////////////////////////////////////////
// [getfreespace]
//////////////////////////////////////////////////////////////////////////////////
int C_File_System::getfreespace(const char* psPath, unsigned long* pFree){
  
   struct statvfs stat;
  
   if(!psPath || !pFree) return(C_FILE_SYSTEM_ERROR);
  
   if(statvfs(psPath, &stat) != 0) return(C_FILE_SYSTEM_ERROR);
  
   (*pFree) = (unsigned long)stat.f_bavail * stat.f_frsize;
  
   return(C_FILE_SYSTEM_READY); 
}
