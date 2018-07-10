//To compile: g++ -o3 -w lcovingtAssignment1Serial.cpp -lpthread -o lcovingtAssignment1Serial
//To run: ./lcovingtAssignment1Serial filePath K

//this code contains example code from assign1Graph.cpp written by Saeed Salem

//this code assumes that graphs passed do not posses reflexive edges I.E. you may not be friends with yourself

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int numberOfNodes;
int k;

typedef vector<vector<int> > AdjacencyMatrix;

AdjacencyMatrix adjMatrix;

AdjacencyMatrix commonFriendCounts;

AdjacencyMatrix  sortedRecommendationList;//list that used commonFriendCounts to sort ones friends list in Descending order so it is easy to print friend recommendations


void initialize(char* filePath, int numberOfRecommendationsToMake){//initialize all variables to support our calculations for recommendation lists

    k = numberOfRecommendationsToMake;

    fstream myfile(filePath,std::ios_base::in);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;
    while(myfile >> u >> v)
    {
        allEdges.push_back(make_pair(u,v));
        if(u > maxNode)
          maxNode = u;

        if(v > maxNode)
          maxNode = v;
    }

    numberOfNodes = maxNode +1;  //Since nodes starts with 0
    //cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(numberOfNodes,vector<int>(numberOfNodes));
    commonFriendCounts = AdjacencyMatrix(numberOfNodes,vector<int>(numberOfNodes));

    //populate the adjacency matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
    }

    //populate commonFriendCounts with -2 to denote that we have not yet calculated their commonFriendValues
    for(int i =0; i < commonFriendCounts.size(); i++){
	for(int j = 0; j < commonFriendCounts[j].size(); j++){
	    commonFriendCounts[i][j] = -2;//we write -2 to represent that this calculation has not been made yet
	}
    }

    sortedRecommendationList = AdjacencyMatrix(numberOfNodes,vector<int>(0));

}

bool isFriend(int person1, int person2){

    if(adjMatrix[person1][person2] == 1){
	return true;
    }

    return false;

}


void calcAndSaveNumberOfSharedCommonFriends(int person1, int person2){

    if(commonFriendCounts[person1][person2] != -2){
	return;
    }
    else if(isFriend(person1, person2)|| person1 == person2){//if they are already friends or are the same person write a -1 in their common friends counts, this is so when we sort our rec list they will be at the bottom
	commonFriendCounts[person1][person2] = -1;
	commonFriendCounts[person2][person1] = -1;
	return;
    }

    int commonFriendCounter = 0;
    for(int i =0; i < numberOfNodes; i++){
	if(isFriend(person1,i) && isFriend(person2,i)){
	    commonFriendCounter = commonFriendCounter + 1;
	}
    }

    commonFriendCounts[person1][person2] = commonFriendCounter;
    commonFriendCounts[person2][person1] = commonFriendCounter;


}

void printVector(vector<int> v){
    for(int i = 0; i < v.size(); i++){
	cout << v[i];
    }
    cout << "\n";
    return;
}

void createRecommendationList(int person){

    //initialize a list of candidates, as we perform a max search for common friends we will remove them from this temp list and add them to the recommendation list
    vector<int> candidatesForRecommendation = vector<int>(numberOfNodes);
    for(int i = 0; i < candidatesForRecommendation.size(); i++){
        candidatesForRecommendation[i] = i;
    }

    candidatesForRecommendation.erase(candidatesForRecommendation.begin() + person);//remove person as we don't want to recommend someone to themselves, we want to find other friends for them

    for(int i = 0; i < k; i++){//for k recommendations we want in our list

        int indexToBestCandidateSoFar = 0;

        for(int z = 1; z < candidatesForRecommendation.size(); z++){//for all the candidates we have find the one with the most common friends with person
            if(commonFriendCounts[person][candidatesForRecommendation[z]] > commonFriendCounts[person][candidatesForRecommendation[indexToBestCandidateSoFar]]){
                indexToBestCandidateSoFar = z;
            }
        }

        int friendToRecommend = candidatesForRecommendation[indexToBestCandidateSoFar];

        for(int j = 0; j < candidatesForRecommendation.size(); j++){//delete the new recommended friend from our candidate list
            if(friendToRecommend == candidatesForRecommendation[j]){
                candidatesForRecommendation.erase(candidatesForRecommendation.begin() + j);
            }
        }

        if(commonFriendCounts[person][friendToRecommend] > -1){//if the best recommendation we can come up with is already a friend with them
            sortedRecommendationList[person].push_back(friendToRecommend);//save the new recommended friend to our recommendation list
        }

    }

}


void printRecommendation(int person){//access the descending

    cout << "Friend Recommendations for: " << person << ": ";

    for(int i = 0; i < sortedRecommendationList[person].size(); i++){
	cout << sortedRecommendationList[person][i] << ",";
    }

    cout << "\n";

}

//methods for debugging
void printAdjMatrix(AdjacencyMatrix matrix){

    for(int i =0; i < matrix.size(); i++){
	for(int j = 0; j < matrix.size(); j++){
	    cout << matrix[i][j];
	}
	cout << "\n";
    }

}

void printLine(){
    cout << "\n";
}


void runProgram(char* filePath, int numberOfRecommendationsToMake){

    initialize(filePath, numberOfRecommendationsToMake);

    //calculate common friend counts
    for(int i = 0; i < numberOfNodes; i++){
	for(int j = 0; j < numberOfNodes; j++){
	    calcAndSaveNumberOfSharedCommonFriends(i,j);
       }
    }

    //create recommendation list of size k based on common friend counts
    for(int i = 0; i < numberOfNodes; i++){
	createRecommendationList(i);
    }

    //print all friend recommendations
    for(int i = 0; i < numberOfNodes; i++){
	printRecommendation(i);
    }



}

int main(int argc, char** argv)
{
    runProgram(argv[1],atoi(argv[2]));

}
