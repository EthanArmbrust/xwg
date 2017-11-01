#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

char * insertGarbage(char * fileContents, int filesize, int bytesPerBlock, int maxBlockSize, char garbageSignifier);

std::vector<char> deleteGarbage(char * fileContents, int filesize, char garbageSignifier);

char * xorCharArray(char * input, int size, const std::string& password);

char * fileReader(const std::string& filename);

void fileWriter(char * fileContents, int size, const std::string& outFilename);

void fileWriter(vector<char> inContents, const std::string& outFilename);

int getReadFileSize();

int getGarbageSize();


string getOption(string optionName, int argc, char * argv[]){
  for(int i = 0; i < argc;i++){
    if(argv[i] == optionName){
      if(i < argc -1){
         return argv[i+1];
      }
      else return "__BAD_OPTION__";
    }
  }
  return "__OPTION_NOT_FOUND__";
}


int main(int argc, char * argv[]){

  if(argc > 3){

  string mode = argv[1];
  string filename;
  string password;

  int maxGarbageSize = 256;
  char garbageSignifier = '~';

  string sBytesPerBlock = getOption("--bytes-per-block", argc, argv);
  int iBytesPerBlock = 10;
  if(sBytesPerBlock != "__BAD_OPTION__" && sBytesPerBlock != "__OPTION_NOT_FOUND__"){		
  	iBytesPerBlock = stoi(sBytesPerBlock);
  }

  string possiblePassword = getOption("--password", argc,argv);

  if(possiblePassword != "__BAD_OPTION__" && possiblePassword != "__OPTION_NOT_FOUND"){
	password = possiblePassword;
  }
  else {
    if(mode != "defile" && mode != "clean"){
	cout << "No password givin.  Password required." << endl;
	return 1;
    }
  }


  
  string possibleIn = getOption("--input", argc, argv);
  if(possibleIn != "__BAD_OPTION__" && possibleIn != "__OPTION_NOT_FOUND__"){
	filename = possibleIn;
  }
  else{
	cout << "No input givin. Input required." << endl;
	return 1;
  }

  string outputFilename = filename + "-out";
  string possibleOut = getOption("--output", argc, argv);
  if(possibleOut != "__BAD_OPTION__" && possibleOut != "__OPTION_NOT_FOUND__"){
	outputFilename = possibleOut;
  }

  string sMaxBlockSize = getOption("--max-block-size", argc, argv);
  if(sMaxBlockSize != "__BAD_OPTION__" && sMaxBlockSize != "__OPTION_NOT_FOUND__"){		
  	maxGarbageSize = stoi(sMaxBlockSize);
  }
  
  string garbageSeparator = getOption("--garbage-signifier",argc, argv);
  if(garbageSeparator != "__BAD_OPTION__" && garbageSeparator != "__OPTION_NOT_FOUND__"){
	garbageSignifier = garbageSeparator[0];
  }


  if(mode == "defile"){
    char * oc = fileReader(filename);
    char * garbo = insertGarbage(oc, getReadFileSize(), iBytesPerBlock, maxGarbageSize, garbageSignifier);
    fileWriter(garbo,getGarbageSize(), outputFilename);
    delete[] oc;
    delete[] garbo;
  }
  if(mode == "clean"){ 
    cout << "garbage signifier: " << garbageSignifier << endl;
    char * oc = fileReader(filename);
    vector<char> cleaned = deleteGarbage(oc, getReadFileSize(), garbageSignifier);
    fileWriter(cleaned, outputFilename);
    delete[] oc;
  }
  if(mode == "encrypt-garbage"){
    cout << "garbage signifier: " << garbageSignifier << endl;
    char * oc = fileReader(filename);
    char * garbo = insertGarbage(oc, getReadFileSize(), iBytesPerBlock, maxGarbageSize, garbageSignifier);
    char * xord = xorCharArray(garbo, getGarbageSize(), password);
    fileWriter(xord, getGarbageSize(), outputFilename);
    delete[] oc;
    delete[] garbo;
    delete[] xord;
  }
  if(mode == "decrypt-garbage"){ 
    cout << "garbage signifier: " << garbageSignifier << endl;
    char * oc = fileReader(filename);
    char * xord = xorCharArray(oc, getReadFileSize(), password);
    vector<char> cleaned = deleteGarbage(xord, getReadFileSize(), garbageSignifier);
    fileWriter(cleaned, outputFilename);
    delete[] oc;
    delete[] xord;
  }
  if(mode == "encrypt" || mode == "decrypt"){
    char * oc = fileReader(filename);
    char * xord = xorCharArray(oc, getReadFileSize(), password);
    fileWriter(xord, getReadFileSize(), outputFilename);
    delete[] oc;
    delete[] xord;
  }
  }
  else {
	cout << "usage: xor-encryptor [mode] file password" << endl;
  }

  return 0;
}
