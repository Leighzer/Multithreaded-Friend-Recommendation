//CSCI415; Saeed Salem, 9/6/2017
//To compile: g++ -O3 -w assign1Graph.cpp -lpthread -o assign1Graph
//To run: ./assign1Graph filename k
// ./assign1Graph networkDatasets/toyGraph1.txt 2

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;


void printAdjMatrix(AdjacencyMatrix adjMatrix)
{
    for (int i=0; i<adjMatrix.size(); i++)
    {
        for (int j=0; j<adjMatrix[i].size(); j++) cout<<adjMatrix[i][j]<<" ";
        cout<<endl;
    }

}

int main(int argc, char** argv)
{
    if(argc<3){
      cout<<"To run: ./assign1Graph filename k"<<endl;
      cout<<"./assign1Graph networkDatasets/toyGraph1.txt 2"<<endl;
      return 0;
    }
      
    fstream myfile(argv[1],std::ios_base::in);
    int k = atoi(argv[2]);
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

    int n = maxNode +1;  //Since nodes starts with 0
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
    }
   
    if(n<=10) 
      printAdjMatrix(adjMatrix);
    
	//You can also make a list of neighbors for each node if you want.
 
return 0;
}
