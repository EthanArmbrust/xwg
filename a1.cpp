#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

int totalGarbagedSize = 0;

int readFileSize = -1;



char * insertGarbage(char * fileContents, int filesize, int bytesPerBlock, int maxGarbageSize, char garbageSignifier);

vector<char> deleteGarbage(char * fileContents, int filesize, char garbageSignifier);

char randomChar();



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
  return (char) (rand() % 256);
}

vector<char> deleteGarbage(char * fileContents, int filesize, char garbageSignifier){

   vector<char> deletedGarbage;
   bool inGarbage = false;
   for(int f = 0; f < filesize; f++){
	if(fileContents[f] == garbageSignifier){
	    inGarbage = !inGarbage;
	}
	if(!inGarbage && fileContents[f] != garbageSignifier){
	   deletedGarbage.push_back(fileContents[f]);
	}
   }
   return deletedGarbage;

}

int getGarbageSize(){
  return totalGarbagedSize;
}
int getReadFileSize(){
  return readFileSize;
}
