#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

int totalGarbagedSize = 0;

int readFileSize = -1;

int get_file_size(const std::string& filename);
char * insertGarbage(char * fileContents, int filesize, int bytesPerBlock, int maxGarbageSize, char garbageSignifier);
vector<char> deleteGarbage(char * fileContents, int filesize);
char randomChar();




void encrypt_file(const std::string& filename, const std::string& password) {
   int x = 12;
   int& ref = x;
   streampos size;
   char * fileContents;
   char * newFileContents;

   ifstream inFile(filename, ios::in|ios::binary|ios::ate);

   size = inFile.tellg();
   fileContents = new char[size];
   newFileContents = new char[size];

   inFile.seekg(0, ios::beg);
   inFile.read(fileContents, size);
   inFile.close();

   for(int i = 0; i < (int) size; i++){
	newFileContents[i] = fileContents[i] ^ password[i % password.length()];
   }

   ofstream outFile(filename + "-encrypted", ios::out|ios::binary);
   outFile.write(newFileContents, size);
   outFile.close();

   delete[] fileContents;
   delete[] newFileContents;

}

std::string crack_file(const std::string& originalFilename, const std::string& encryptedFilename) {

    streampos size;
    char * originalContents;
    char * encryptedContents;
    char * passwordList;

    ifstream originalFile(originalFilename, ios::in|ios::binary|ios::ate);

    size = originalFile.tellg();
    originalContents = new char[size];
    encryptedContents = new char[size];
    passwordList = new char[size];

    originalFile.seekg(0, ios::beg);
    originalFile.read(originalContents, size);
    originalFile.close();

    ifstream encryptedFile(encryptedFilename, ios::in|ios::binary);

    encryptedFile.read(encryptedContents, size);
    encryptedFile.close();

    for(int i = 0; i < (int) size; i++){
	passwordList[i] = originalContents[i] ^ encryptedContents[i];
    }

    //printf("password list:%s\n", passwordList);

    for(int i = 1; i < ((int) size) / 2; i++){
	string str1 = "";
	string str2 = "";
	for(int z = 0; z < i; z++){
	   str1 += passwordList[z];
	   str2 += passwordList[i+z];
	}
	//printf("str1:%s\nstr2:%s\n", str1.c_str(), str2.c_str());
	if(str1 == str2){
	   delete[] originalContents;
	   delete[] encryptedContents;
	   delete[] passwordList;
	   printf("password:%s",str1.c_str());
	   return str1;
	}
    }

    string stringList = "";

    for(int t = 0; t < (int) size; t++){
	stringList += passwordList[t];
    }

    //printf("stringList:%s\n", stringList.c_str());
    //printf("size:%i\n", (int) size);
    delete[] originalContents;
    delete[] encryptedContents;
    delete[] passwordList;

    return stringList;
}

char * insertGarbage(char* fileContents, int filesize, int bytesPerBlock, int maxBlockSize, char garbageSignifier){
    srand(time(NULL));

    int numOfBlocks = filesize / bytesPerBlock;
    vector<char *> charGarbageVector;
    vector<int> blockSizes;
    vector<int> garbageLocations;

    int mySum = 0;
    for(int i = 0; i < numOfBlocks; i++){
	int garbageSize = (rand() % maxBlockSize) + 2; //add two for ~begin and end~
	char * garbageBlock = new char[garbageSize];
	blockSizes.push_back(garbageSize);
	mySum += garbageSize;
	garbageBlock[0] = garbageSignifier;
	garbageBlock[garbageSize - 1] = garbageSignifier; //set begin and end signifiers

	for(int z = 1; z < garbageSize -1; z++){
	    char randChar = randomChar();
	    while(randChar == garbageSignifier){     //dont want any stray garbageSignifiers in there
		randChar = randomChar();
	    }
	    garbageBlock[z] = randChar;
	}
	charGarbageVector.push_back(garbageBlock);
    }

    int garbageSum = 0;
    for(int y = 0; y < blockSizes.size(); y++){
	garbageSum += blockSizes[y];
    }

    char * returnVector = new char[filesize + garbageSum];

    for(int g = 0; g < numOfBlocks; g++){
	int randomInt = rand() % filesize;
	while(find(garbageLocations.begin(),garbageLocations.end(),randomInt) != garbageLocations.end()){
	   randomInt = rand() % filesize;
	}
	garbageLocations.push_back(randomInt);
    }

    sort(garbageLocations.begin(), garbageLocations.end());

    int originalContentCursor = 0;
    int locationCursor = 0;
    int p = 0;		//new contents cursor

    while(p < filesize + garbageSum){
	if(originalContentCursor == garbageLocations[locationCursor]){
	   for(int z = 0; z < blockSizes[locationCursor]; z++){
		returnVector[p] = charGarbageVector[locationCursor][z];
		p++;
	   }
	   locationCursor++;
	}
 	returnVector[p] = fileContents[originalContentCursor];
	p++;
	originalContentCursor++;
    }


   totalGarbagedSize = filesize + garbageSum;
   return returnVector;
}

void garbageInserter(const std::string& filename){
   streampos size;
   char * fileContents;
   char * newFileContents;

   ifstream inFile(filename, ios::in|ios::binary|ios::ate);

   size = inFile.tellg();
   fileContents = new char[size];


   inFile.seekg(0, ios::beg);
   inFile.read(fileContents, size);
   inFile.close();

   //printf("read file\n");

   newFileContents = insertGarbage(fileContents, size, 2, 256, '~');

   int newFileSize = sizeof(newFileContents) / sizeof(newFileContents[0]);
   size_t len = strlen(newFileContents);
   //printf("sizeof newFileContents: %lu\n", sizeof(newFileContents));
   //printf("sizeof newFileContents[0]: %lu\n", sizeof(newFileContents[0]));
   //printf("strlen of newFileContents: %lu\n",len);

   //printf("newFileSize: %d\n", newFileSize);
   ofstream outFile(filename + "-fouled", ios::out|ios::binary);
   outFile.write(newFileContents, totalGarbagedSize);
   outFile.close();

   //printf("wrote file\n");
   delete[] fileContents;
   delete[] newFileContents;
}

void garbageDeleter(const std::string& filename){
   streampos size;
   char * fileContents;
   char * newFileContents;

   ifstream inFile(filename, ios::in|ios::binary|ios::ate);

   size = inFile.tellg();
   fileContents = new char[size];


   inFile.seekg(0, ios::beg);
   inFile.read(fileContents, size);
   inFile.close();

   //printf("read file\n");

   vector<char> garbageBeGone = deleteGarbage(fileContents, size);

   newFileContents = new char[garbageBeGone.size()];

   for(int i = 0; i < garbageBeGone.size(); i++){
	     newFileContents[i] = garbageBeGone[i];
   }


   ofstream outFile(filename + "-cleaned", ios::out|ios::binary);
   outFile.write(newFileContents, garbageBeGone.size());
   outFile.close();

   //printf("wrote file\n");
   delete[] fileContents;
   delete[] newFileContents;
}

char * fileReader(const std::string& filename){
   streampos size;
   char * fileContents;

   ifstream inFile(filename, ios::in|ios::binary|ios::ate);
   size = inFile.tellg();
   fileContents = new char[size];

    inFile.seekg(0, ios::beg);
    inFile.read(fileContents,size);
    inFile.close();

    readFileSize = size;

    return fileContents;
}

void fileWriter(char * fileContents, int size, const std::string& outFilename){
	ofstream outFile(outFilename, ios::out|ios::binary);
	outFile.write(fileContents, size);
	outFile.close();
}

void fileWriter(vector<char> inContents, const std::string& outFilename){
	char * fileContents = new char[inContents.size()];

	for(int a = 0; a < inContents.size(); a++){
	    fileContents[a] = inContents[a];
	}

	ofstream outFile(outFilename, ios::out|ios::binary);
	outFile.write(fileContents, inContents.size());
	outFile.close();
}

char * xorCharArray(char * input, int size, const std::string& password){
	char * encrypted = new char[size];

	for(int i = 0; i < (int) size; i++){
	    encrypted[i] = input[i] ^ password[i % password.length()];
   	}
	return encrypted;

}

char randomChar(){
    return (char) (rand() % 128);
}

vector<char> deleteGarbage(char * fileContents, int filesize){

   vector<char> deletedGarbage;
   bool inGarbage = false;
   for(int f = 0; f < filesize; f++){
	if(fileContents[f] == '~'){
	    inGarbage = !inGarbage;
	}
	if(!inGarbage && fileContents[f] != '~'){
	   deletedGarbage.push_back(fileContents[f]);
	}
   }
   return deletedGarbage;

}

int get_file_size(const std::string& filename){

  ifstream file;
  streampos begin, end;


  file.open(filename, ios::binary);

  begin = file.tellg();
  file.seekg(0, ios::end);
  end = file.tellg();

  file.close();

  return end - begin;
}

int getGarbageSize(){
  return totalGarbagedSize;
}
int getReadFileSize(){
  return readFileSize;
}
