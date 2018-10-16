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
    if (firstLine){ //first line in the file (number of test cases)
      numCases = stoi(parsedRow.at(0));
      firstLine = false;
      firstCaseLine = true;
      caseNumber = 0;
    }
    else if (firstCaseLine){  //first line in the test case
      //cout << "case" << endl;
      numCaseItems = stoi(parsedRow.at(0));
      firstCaseLine = false;
      casePrices = new int[numCaseItems];
      caseWeights = new int[numCaseItems];
      caseItemCounter = 0;
      caseNumber++;
    }
    else if (caseItemCounter < numCaseItems) {  //add item to item array
      //cout << "Item " << caseItemCounter << endl;
      casePrices[caseItemCounter] = stoi(parsedRow.at(0));
      caseWeights[caseItemCounter] = stoi(parsedRow.at(1));
      caseItemCounter++;
    }
    else if (caseItemCounter == numCaseItems){  //get number of family members, initialize array
      numFamilyMembers = stoi(parsedRow.at(0));
      //cout << "Family Members: " << numFamilyMembers << endl;
      caseFamilyMembers = new int[numFamilyMembers];
      caseItemCounter++;
      caseFamilyCounter = 0;
    }
    else if (caseFamilyCounter < numFamilyMembers){ //add weight to family member array
      //cout << "Weight: " << stoi(parsedRow.at(0)) << endl;
      caseFamilyMembers[caseFamilyCounter] = stoi(parsedRow.at(0));
      caseFamilyCounter++;
    }
    if (caseFamilyCounter == numFamilyMembers){ //all familyMembers weights have been read in, execute the optimization
      caseFamilyCounter++;
      firstCaseLine = true; //reset for next case
      //cout << "SHOPPING" << endl;
      fileout.append(shoppingOptimization(caseNumber,numCaseItems,casePrices,caseWeights,numFamilyMembers,caseFamilyMembers));
    }
  }
  //cout << fileout;

  //Write string to file
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
  for (int membercount = 0; membercount < numFamilyMembers; membercount++){ //run for each family member
    int maxWeight = familyMembers[membercount];
    output.append(to_string(membercount+1));
    output.append(":");
    int V[numItems+1][maxWeight+1]; //array for weights and item counts
    bool keep[numItems+1][maxWeight+1]; //whether or not there is an item here

    for (int i = 0; i <= numItems; i++){  //step through Items
      for (int w = 0; w <= maxWeight; w++){ //increase weight until max weight
        if (i == 0 || w == 0){
          V[i][w] = 0;
        }
        else if (weights[i-1] <= w){  //if the weight in the square below is lower than the current max, find the max and keep it
          int a = prices[i-1] + V[i-1][w-weights[i-1]];
          int b = V[i-1][w];
          if (a > b) {V[i][w] = a;}else {V[i][w] = b;}
          keep[i][w] = 1;
        }
        else {  //otherwise, set the current same as the last, and don't keep it
          V[i][w] = V[i-1][w];
          keep[i][w] = 0;
        }
      }
    }

    //walk back through and compile path list
    int res = V[numItems][maxWeight];;
    int k = maxWeight;
    string subString = "";
    for (int i = numItems; i > 0 && res > 0; i--){  //step through the list until there either are no more items or the total has been reached
      if (!(res == V[i-1][k])){
        string temp = subString;  //used to put numbers in order instead of being backwards
        subString = " ";
        //subString.append(" ");
        subString.append(to_string(i));
        subString.append(temp);
        res = res - prices[i-1];
        k = k-weights[i-1];
      }
    }

    //add subString to output
    output.append(subString);
    output.append("\n");
    totalPrice += V[numItems][maxWeight];
  }
  //cout << "Test Case " << caseNumber << "\n";
  //cout << "Total Price " << totalPrice << "\n";
  //cout << "Member Items:\n";
  //cout << output << endl;

  //compile string for output for this test case
  string retstr = "Test Case ";
  retstr.append(to_string(caseNumber));
  retstr.append("\nTotal Price ");
  retstr.append(to_string(totalPrice));
  retstr.append("\nMember Items:\n");
  retstr.append(output);
  retstr.append("\n");
  //return string
  return retstr;
}
