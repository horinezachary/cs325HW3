#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <vector>

using namespace std;

void shoppingOptimization(int caseNumber, int numItems, int* prices, int* weights, int numFamilyMembers, int* familyMembers);

int main(){
  ifstream inFile;  //set up the file reader and attempt to read the file
  inFile.open("shopping.txt");
  if (!inFile) {  //make sure the file exists
    cout << "Unable to open shopping.txt";
    exit(1);   // EXIT PROGRAM
  }
  string line;
  int numCases;
  int caseNumber;
  bool firstLine = true;
  bool firstCaseLine = false;
  int numCaseItems = 0;
  int caseItemCounter = 0;
  int* casePrices;
  int* caseWeights;
  int numFamilyMembers;
  int caseFamilyCounter;
  int* caseFamilyMembers;

  while(getline(inFile,line)){ //gets one line
    stringstream lineStream(line);
    string cell;
    vector<string> parsedRow;
    while(getline(lineStream,cell,' ')){  //parses line and puts numbers into string vector
      parsedRow.push_back(cell);
    }
    if (firstLine){
      numCases = stoi(parsedRow.at(0));
      firstLine = false;
      firstCaseLine = true;
      caseNumber = 0;
    }
    else if (firstCaseLine){
      numCaseItems = stoi(parsedRow.at(0));
      firstCaseLine = false;
      casePrices = new int[numCaseItems];
      caseWeights = new int[numCaseItems];
      caseItemCounter = 0;
      caseNumber++;
    }
    else if (caseItemCounter < numCaseItems) {
      casePrices[caseItemCounter] = stoi(parsedRow.at(0));
      caseWeights[caseItemCounter] = stoi(parsedRow.at(1));
      caseItemCounter++;
    }
    else if (caseItemCounter == numCaseItems){
      numFamilyMembers = stoi(parsedRow.at(0));
      caseFamilyMembers = new int[numFamilyMembers];
      caseFamilyCounter = 0;
    }
    else if (caseFamilyCounter < numFamilyMembers){
      caseFamilyMembers[caseFamilyCounter] = stoi(parsedRow.at(0));
      caseFamilyCounter++;
    }
    else{
      shoppingOptimization(caseNumber,numCaseItems,casePrices,caseWeights,numFamilyMembers,caseFamilyMembers);
      firstCaseLine = true;
    }
  }
}
void shoppingOptimization(int caseNumber, int numItems, int* prices, int* weights, int numFamilyMembers, int* familyMembers){
  int totalPrice = 0;
  string output = "";
  for (int membercount = 0; membercount < numFamilyMembers; membercount++){
    int maxWeight = familyMembers[membercount];
    output.append(to_string(membercount+1));
    output.append(":");
    int V[numItems][maxWeight];
    bool keep[numItems][maxWeight];
    for (int w = 0; w < maxWeight; w++){
      V[0][w] = 0;
    }
    for (int i = 1; i < numItems; i++){
      for (int w = 0; w < maxWeight; w++){
        if ((weights[i] <= w) && (prices[i] + V[i-1][w-weights[i]] > V[i-1][w])){
          V[i][w] = prices[i] + V[i-1][w-weights[i]];
          keep[i][w] = true;
        }
        else {
          V[i][w] = V[i-1][w];
          keep[i][w] = false;
        }
      }
    }
    int K = maxWeight;
    for (int i = numItems; i >= 1; i--){
      if (keep[i][K] == true){
        output.append(" ");
        output.append(to_string(i));
        K = K-weights[i];
      }
    }
    output.append("\n");
    totalPrice += V[numItems][maxWeight];
  }
  cout << "Test Case " << caseNumber << "\n";
  cout << "Total Price " << totalPrice << "\n";
  cout << "Member Items:\n";
  cout << output << endl;
}
