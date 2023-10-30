#include <iostream>
#include <fstream>

//Open file of this name
ifstream myFile;
myFile.open("myFileName.fileExt");
//Check for errors
if(myFile.fail()){
	cerr << "Error - Failed to open " << filename << endl;
}
//Take text in file

fin.close( );

string fileText = "";

//For every line, add it to the text
while(getline(myFile, line)){
	fileText += line;
}

//Convert string to char* (C-style strings)
char* shaderText = fileText.c_str();

string LoadShader(string fileName) {
ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
    	cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
    	fileText += line + '\n';
	}
    
	myFile.close();
	return fileText;
}
