#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <queue>

using namespace std;

vector< vector<int> > fileToMatrix(string filename){
    char delimiter = ' ';

    // feature vector
    vector<int> feature;

    // Matrix to hold all feature vecttors
    vector< vector<int> > featureMatrix;

    // Variable to hold char number from file
    char line[4];

    // Variable to hold number that used to be a char
    int number;

    // Counter to check when to put into featureMatrix
    int count = 0;

    ifstream trainfile; 
    trainfile.open(filename);

    //check to see if the file is opened:
    if (trainfile.is_open())
    {
        //while there are still lines in the
        //file, keep reading:
        while (! trainfile.eof() )
        {
            count++;

            // Get character from file and store in variable line
            trainfile.getline(line, 256, delimiter);

            // line is a character so convert to int
            number = atoi(line);

            // add to feature vector
            feature.push_back(number);

            // If counter is 785, then feature vector has been filled and you can add it to matrix
            if(count == 785){
                featureMatrix.push_back(feature);

                // reset values
                count = 0;
                feature.clear();
            } 

        }     
    }
    trainfile.close();

    return featureMatrix;
}

int main() {

    string trainingFile = "hw2train.txt";
    string validateFile = "hw2validate.txt";
    string testFile = "hw2test.txt";

    vector< vector<int> > trainingMatrix = fileToMatrix(trainingFile);
    vector< vector<int> > validateMatrix = fileToMatrix(validateFile);
    vector< vector<int> > testMatrix = fileToMatrix(testFile);

    cout << "This is the error: " << value << endl;
   
    return 0;
}