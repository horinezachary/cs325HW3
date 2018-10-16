#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <vector>

using namespace std;

string shoppingOptimization(int caseNumber, int numItems, int* prices, int* weights, int numFamilyMembers, int* familyMembers);

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
  bool exec = false;
  int numCaseItems = 0;
  int caseItemCounter = 0;
  int* casePrices;
  int* caseWeights;
  int numFamilyMembers;
  int caseFamilyCounter;
  int* caseFamilyMembers;
  int linecount = 1;
  string fileout = "";
  while(getline(inFile,line)){ //gets one line
    //cout << "LINE: " << linecount << endl;
    linecount++;
    stringstream lineStream(line);
    string cell;
    vector<string> parsedRow;
    while(getline(lineStream,cell,' ')){  //parses line and puts numbers into string vector
      parsedRow.push_back(cell);
      //cout << cell;
    }
    //cout << endl;
    if (firstLine){
      numCases = stoi(parsedRow.at(0));
      firstLine = false;
      firstCaseLine = true;
      caseNumber = 0;
    }
    else if (firstCaseLine){
      if (exec){
        //cout << "SHOPPING" << endl;
        fileout.append(shoppingOptimization(caseNumber,numCaseItems,casePrices,caseWeights,numFamilyMembers,caseFamilyMembers));
      }
      //cout << "case" << endl;
      numCaseItems = stoi(parsedRow.at(0));
      firstCaseLine = false;
      casePrices = new int[numCaseItems];
      caseWeights = new int[numCaseItems];
      caseItemCounter = 0;
      caseNumber++;
    }
    else if (caseItemCounter < numCaseItems) {
      //cout << "Item " << caseItemCounter << endl;
      casePrices[caseItemCounter] = stoi(parsedRow.at(0));
      caseWeights[caseItemCounter] = stoi(parsedRow.at(1));
      caseItemCounter++;
    }
    else if (caseItemCounter == numCaseItems){
      numFamilyMembers = stoi(parsedRow.at(0));
      //cout << "Family Members: " << numFamilyMembers << endl;
      caseFamilyMembers = new int[numFamilyMembers];
      caseItemCounter++;
      caseFamilyCounter = 0;
    }
    else if (caseFamilyCounter < numFamilyMembers){
      //cout << "Weight: " << stoi(parsedRow.at(0)) << endl;
      caseFamilyMembers[caseFamilyCounter] = stoi(parsedRow.at(0));
      caseFamilyCounter++;
    }
    if (caseFamilyCounter == numFamilyMembers){
      caseFamilyCounter++;
      firstCaseLine = true;
      exec = true;
    }
  }
  //cout << "SHOPPING" << endl;
  fileout.append(shoppingOptimization(caseNumber,numCaseItems,casePrices,caseWeights,numFamilyMembers,caseFamilyMembers));

  ofstream outfile;
  outfile.open("shopping.out");
  if (!outfile) {  //make sure the file exists
    cout << "Unable to open insert.out";
    exit(1);   // EXIT PROGRAM
  }
  outfile << fileout;
  outfile.close();
}

string shoppingOptimization(int caseNumber, int numItems, int* prices, int* weights, int numFamilyMembers, int* familyMembers){
  int totalPrice = 0;
  string output = "";
  for (int membercount = 0; membercount < numFamilyMembers; membercount++){
    int maxWeight = familyMembers[membercount];
    output.append(to_string(membercount+1));
    output.append(":");
    int V[numItems+1][maxWeight+1];
    bool keep[numItems+1][maxWeight+1];

    for (int i = 0; i <= numItems; i++){
      for (int w = 0; w <= maxWeight; w++){
        if (i == 0 || w == 0){
          V[i][w] = 0;
        }
        else if (weights[i-1] <= w){
          int a = prices[i-1] + V[i-1][w-weights[i-1]];
          int b = V[i-1][w];
          int max;
          if (a >= b) {max = a;}
          if (a < b) {max = b;}
          V[i][w] = max;
          keep[i][w] = 1;
        }
        else {
          V[i][w] = V[i-1][w];
          keep[i][w] = 0;
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

  string retstr = "Test Case ";
  retstr.append(to_string(caseNumber));
  retstr.append("\nTotal Price ");
  retstr.append(to_string(totalPrice));
  retstr.append("\nMember Items:\n");
  retstr.append(output);
  retstr.append("\n");

  return retstr;
}
