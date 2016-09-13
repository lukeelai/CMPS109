// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $
//Gabriel Reyes ghreyes
//Luke Lai luelai

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

//using str_str_map = listmap<string,string>;
//using pair = xpair<string,string>;
using xpair_map = listmap<string, string>;
//using str_str_pair = str_str_map::value_type; // pair = ((string, string), value_type)

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);

   xpair_map test;
   //if argv contains files
   if(argv[1] != NULL && (argc > 1 || (argc >0 && (string)argv[1] != "-@"))){
      //ifstream readFile;
      //iterates through argv (filenames)
      for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
         ifstream readFile((string)*argp);
         int lineCount = 1;
         string currLine;
         if(readFile.good()){
            //this loop reads through file, parsing lines into string currLine
            while(getline(readFile, currLine)){
               
               //getline removes \n
               //this is to remove \r
               //http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
               if ( currLine.size() && currLine[currLine.size()-1] == '\r' ) {
                  currLine = currLine.substr( 0, currLine.size() - 1 );
               }

               //removes whitespace at front and back of currLine
               //http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
               currLine.erase(0, currLine.find_first_not_of(' '));  
               currLine.erase(currLine.find_last_not_of(' ')+1);

               //line is a comment
               if(currLine[0] == '#' || currLine == ""){
                  //comment or blank line does nothing
               }

               else{
                  //split line into key and value
                  size_t pos = currLine.find("=");
                  string key = currLine.substr(0,pos);
                  string value = currLine.substr (pos+1);

                  //remove whitespace from ends of key and value
                  key.erase(key.find_last_not_of(' ')+1);
                  value.erase(0, value.find_first_not_of(' '));  

                  cout << *argp << ": " << lineCount << ": " << currLine << endl;
                  
                  //if no "="
                  if(pos > currLine.length()){
                     if(test.find(value) == test.end()){
                        cerr << value << ": " << "key not found" << endl;
                     }
                     else cout << *(test.find(value))<< endl;
                  }
                  //just an "="
                  else if(key.length() == 0 && value.length() == 0){
                     for (xpair_map::iterator itor = test.begin();
                        itor != test.end(); ++itor) {
                        cout << *itor << endl;
                     }
                  }

                  //print all pairs with this value
                  else if(key.length() == 0 && value.length() != 0){
                     for (xpair_map::iterator itor = test.begin();
                        itor != test.end(); ++itor){
                        if(itor->second == value){ 
                           cout << *itor << endl;
                        }
                     }
                  }
                  //delete pair
                  else if(key.length() != 0 && value.length() == 0){
                     bool found = false;
                     for (xpair_map::iterator itor = test.begin();
                        itor != test.end(); ++itor){
                        if(itor->first == key){
                           test.erase(itor);
                           found = true;
                           break;
                        }
                     }
                     if(found == false) cout << key << ": not found" << endl;
                  }

                  //set key to value
                  else{
                     //if key exists, replace
                     bool exists = false;
                     for (xpair_map::iterator itor = test.begin();
                        itor != test.end(); ++itor){
                        if(itor->first == key){
                           itor->second = value;
                           cout << *itor << endl;
                           exists = true;
                           break;
                        }
                     }

                     if(!exists){
                        xpair<const string, string> newPair(key,value);
                        cout << *(test.insert(newPair)) << endl;  
                     }//end of if(!exists)  
                 
                  }//end of else
                  lineCount++;

               }//end of else
            }//end of while
         }//end of if
      }//end of for
   }//end of if

   //no files. Code for cin. Bulk of code copied from above
   else{
      //CHANGE THIS TO CIN
      int lineCount = 1;
      string currLine;
      while(getline(cin, currLine)){

         //getline removes \n
         //this is to remove \r
         //http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    	    if ( currLine.size() && currLine[currLine.size()-1] == '\r' ) {
            currLine = currLine.substr( 0, currLine.size() - 1 );
         }

         //removes whitespace at front and back of currLine
         //http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
         currLine.erase(0, currLine.find_first_not_of(' '));  
         currLine.erase(currLine.find_last_not_of(' ')+1);

         //line is a comment
         if(currLine[0] == '#'|| currLine == ""){
            //comments or blank spaces do nothing
         }

         else{
            //split line into key and value
            size_t pos = currLine.find("=");
            string key = currLine.substr(0,pos);
            string value = currLine.substr (pos+1);

            //remove whitespace from ends of key and value
            key.erase(key.find_last_not_of(' ')+1);
            value.erase(0, value.find_first_not_of(' '));  

            cout << "-: " << lineCount << ": " << currLine << endl;
                  
            //if no "="
            if(pos > currLine.length()){
                if(test.find(value) == test.end()){
                    cerr << value << ": " << "key not found" << endl;
                }
                else cout << *(test.find(value))<< endl;
            }

            //just an "="
            else if(key.length() == 0 && value.length() == 0){
               for (xpair_map::iterator itor = test.begin();
                  itor != test.end(); ++itor) {
                  cout << *itor << endl;
               }
            }

            //print all pairs with this value
            else if(key.length() == 0 && value.length() != 0){
               for (xpair_map::iterator itor = test.begin();
                  itor != test.end(); ++itor){
                  if(itor->second == value){ 
                     cout << *itor << endl;
                  }
               }
            }
            //delete pair
            else if(key.length() != 0 && value.length() == 0){
               bool found = false;
               for (xpair_map::iterator itor = test.begin();
                  itor != test.end(); ++itor){
                  if(itor->first == key){
                     test.erase(itor);
                     found = true;
                     break;
                  }
               }
               if(found == false) cout << key << ": not found" << endl;
            }

            //set key to value
            else{
               //if key exists, replace
               bool exists = false;
               for (xpair_map::iterator itor = test.begin();
                  itor != test.end(); ++itor){
                  if(itor->first == key){
                      itor->second = value;
                      cout << *itor << endl;
                      exists = true;
                      break;
                  }
               }
               if(!exists){
                  xpair<const string, string> newPair(key,value);
                  cout << *(test.insert(newPair)) << endl;  
               }//end of if(!exists)                   
            }//end of else
            lineCount++;
         }//end of else
      }//end of while
   }//end of else

   //used to erase map using itor
   xpair_map::iterator itor = test.begin();
   while(itor != test.end()){
      itor = test.erase(itor);
   }

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}
