#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>

using namespace std;

vector<int> vectorAdd(vector<int> v1, vector<int> v2){
    vector<int> sum(784, 0);
    for(int i = 0; i < 784; i++){
        sum[i] = v1[i] + v2[i];
    }
    return sum;
}

vector<long long int> vectorAdd2(vector<long long int> v1, vector<int> v2){
    vector<long long int> sum(784, 0);
    for(int i = 0; i < 784; i++){
        sum[i] = v1[i] + v2[i];
    }
    return sum;
}

vector<int> vectorMul(vector<int> v1, int scalar){
    vector<int> mul(784, 0);
    for(int i = 0; i < 784; i++){
        mul[i] = v1[i] * scalar;
    }
    return mul;
}

int labelSet(int target, int currLabel){
    if(currLabel == target){
        return 1;
    }
    else{
        return -1;
    } 
}

vector<int> perceptron(vector< vector<int> > matrix, int passes, int target){
    vector<int> w(784, 0);
    int dot_product = 0;
    int label = 0;

    for(int p = 0; p < passes; p++){
        for(int i = 0; i < matrix.size(); i++){
            label = labelSet(target, matrix[i][784]);
            dot_product = inner_product(w.begin(), w.end(), matrix[i].begin(), 0);

            if(dot_product*label <= 0){
                w = vectorAdd(w, vectorMul(matrix[i], label)); 
            }
        }
    }
    return w;
}

//creates a list(vector) of W classifier vectors seperating out each label (label is +, other labels are -)
vector< vector<int> > ClassifierCi( vector< vector<int> > data) {
	vector< vector<int> > listOfWVectors;

	for (int i=0; i < 10; i++ ) {
		listOfWVectors.push_back( perceptron(data, 1, i) );	
	}
	return listOfWVectors;
}


int OnevsAllMulticlass( vector< vector<int> > listofWVectors, vector<int> testX) {
	int numofmatches = 0;
	int dot_product = 0;
	int label = 1;    //label is positive always (checking whether it gets marked as positive)
	int returnlabel = 10;

	for (int i=0;i<listofWVectors.size();i++) {
    	dot_product = inner_product(listofWVectors[i].begin(), listofWVectors[i].end(), testX.begin(), 0);
        
		///greater than, does match
        if(dot_product*label > 0){
			numofmatches++;
			returnlabel = i;		                
		}		
	}

	//10 will be index into confusion array for "Dont Know"
	if(numofmatches>1) {
		return 10;
	}
	else{
		return returnlabel;
	}
}

double perceptronError(vector< vector<int> > testData, vector<int> w, int target){
    double error = 0;
    int dot_product = 0;
    int label = 0;

    for(int i = 0; i < testData.size(); i++){
        label = labelSet(target, testData[i][784]);
        dot_product = inner_product(w.begin(), w.end(), testData[i].begin(), 0);

        if(dot_product*label <= 0){
            error++;
        }
    }
    return error/((double)testData.size());
}

vector< vector<int> > votedPerceptron(vector< vector<int> > matrix, int passes, int target){
    vector<int> w(784, 0);
    int dot_product = 0;
    int label = 0;
    vector< vector<int> > wList;
    for(int p = 0; p < passes; p++){
        for(int i = 0; i < matrix.size(); i++){
            label = labelSet(target, matrix[i][784]);
            dot_product = inner_product(w.begin(), w.end(), matrix[i].begin(), 0);

            if(dot_product*label <= 0){
                w = vectorAdd(w, vectorMul(matrix[i], label)); 
            }

            wList.push_back(w);
        }
    }
    return wList;
}

double votedPerceptronError(vector< vector<int> > testData, vector< vector<int> > wList, int target){
    double error = 0;
    int dot_product = 0;
    int label = 0;
    int same = 0;
    int diff = 0;

    for(int i = 0; i < testData.size(); i++){
        for(int j = 0; j < wList.size(); j++){
            label = labelSet(target, testData[i][784]);
            dot_product = inner_product(wList[j].begin(), wList[j].end(), testData[i].begin(), 0);

            if(dot_product*label <= 0){
                diff++;
            }
            else{
                same++;
            }
        }
        //cout<<"Same:  " << same << endl;
        //cout<<"Diff:  " << diff << endl;
        if(same < diff){
            error++;
        }
        same = 0;
        diff = 0;
    }

    return error/((double)testData.size());
}

vector< vector<int> > avgPerceptron(vector< vector<int> > matrix, int passes, int target){
    vector<int> w(784, 0);
    int dot_product = 0;
    int label = 0;
    vector< vector<int> > wList;
    for(int p = 0; p < passes; p++){
        for(int i = 0; i < matrix.size(); i++){
            label = labelSet(target, matrix[i][784]);
            dot_product = inner_product(w.begin(), w.end(), matrix[i].begin(), 0);

            if(dot_product*label <= 0){
                w = vectorAdd(w, vectorMul(matrix[i], label)); 
            }

            wList.push_back(w);
        }
    }
    return wList;
}

double avgPerceptronError(vector< vector<int> > testData, vector< vector<int> > wList, int target){
    double error = 0;
    long long int dot_product = 0;
    int label = 0;
    vector<long long int> wSum(784, 0);

    for(int j = 0; j < wList.size(); j++){
        wSum = vectorAdd2(wSum, wList[j]);
    }

    for(int i = 0; i < testData.size(); i++){
        label = labelSet(target, testData[i][784]);
        dot_product = inner_product(wSum.begin(), wSum.end(), testData[i].begin(), (long long int) 0);
        //cout<<"wsum:   "<<dot_product<<endl;
        if(dot_product*label <= 0){
            error++;
        }
    }

    return error/((double)testData.size());
}

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
    trainfile.open(filename, std::ifstream::in);

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

    string trainingAFile = "hw4atrain.txt";
    string testAFile = "hw4atest.txt";
    string trainingBFile = "hw4btrain.txt";
    string testBFile = "hw4btest.txt";

    int target = 6;
    int passes = 3;
    double error = 0;

    vector< vector<int> > trainingAMatrix = fileToMatrix(trainingAFile);
    vector< vector<int> > testAMatrix = fileToMatrix(testAFile);
    vector< vector<int> > trainingBMatrix = fileToMatrix(trainingBFile);
    vector< vector<int> > testBMatrix = fileToMatrix(testBFile);
   
    cout<< "Starting perception"<<endl;

    for(int i = 1; i < passes + 1; i++){
        vector<int> original = perceptron(trainingAMatrix, i, target);
        
        error = perceptronError(trainingAMatrix, original, target);
        cout<<"This is the training " << i << " error:  " << error << endl;

        error = perceptronError(testAMatrix, original, target);
        cout<<"This is the test " << i << " error:  " << error << endl;      
    }

    cout<<endl<< "Starting voted perception"<<endl;

    for(int i = 1; i < passes + 1; i++){
        vector< vector<int> > voted = votedPerceptron(trainingAMatrix, i, target);
        error = votedPerceptronError(trainingAMatrix, voted, target);
        cout<<"This is the training " << i << " error:  " << error << endl;

        error = votedPerceptronError(testAMatrix, voted, target);
        cout<<"This is the test " << i << " error:  " << error << endl;  
    }

    cout<<endl<< "Starting average perception" << endl;

    for(int i = 1; i < passes + 1; i++){
        vector< vector<int> > avg = avgPerceptron(trainingAMatrix, i, target);
        error = avgPerceptronError(trainingAMatrix, avg, target);
        cout<<"This is the training " << i << " error:  " << error << endl;

        error = avgPerceptronError(testAMatrix, avg, target);
        cout<<"This is the test " << i << " error:  " << error << endl;  
    }  

    double confusion [11][10];
    double labelCount[10];

    memset(confusion, 0, sizeof(confusion));
    memset(labelCount, 0, sizeof(labelCount));

    cout << endl << "One-vs-all Multiclass" << endl;
    vector< vector<int> > listOfWVectors = ClassifierCi(trainingBMatrix);
    int result = 0;
    for(int i = 0; i<testBMatrix.size(); i++) {
    	result = OnevsAllMulticlass(listOfWVectors, testBMatrix[i]);
    	labelCount[testBMatrix[i][784]] = labelCount[testBMatrix[i][784]] + 1;
    	confusion[result][testBMatrix[i][784]]++;
    }

    cout << "Confusion Matrix" << endl;

    for(int i = 0; i < 11; i++){
        for(int j = 0; j < 10; j++){
            confusion[i][j] = confusion[i][j]/labelCount[j];
            cout << "  " << fixed << setprecision(4) << confusion[i][j];
        }
        cout << endl;
    }

    return 0;
}
