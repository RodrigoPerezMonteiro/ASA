#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <list>
#include <iostream>
#include <iterator>
#include <climits>
#include <sstream>
#include <string>
#include <map>

using namespace std;

////////////////////////////////////////////////////////////////////////
//                              al061                                 //
//                       73138 António Pólvora                        //
//                       73701 Rodrigo Monteiro                       //
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//                              Constants                             //
////////////////////////////////////////////////////////////////////////

#define infinito INT_MAX

////////////////////////////////////////////////////////////////////////
//                             Structures                             //
////////////////////////////////////////////////////////////////////////

vector<vector<int>*> *graph; //grafo
vector<map<int, int>*>* flowThroughSpotList; //mapa
int *previousSpotsList;
int *criticalSpotsList; //grafo dos pontos criticos

////////////////////////////////////////////////////////////////////////
//                          Global Variables                          //
////////////////////////////////////////////////////////////////////////

int numSpots = 0; //pontos
int numConnections = 0; //ligações
int numProblems = 0; //problemas
int numStoppedConnections;
int kInput; //k locais por problema
int stoppedConnections; //ligações barradas

////////////////////////////////////////////////////////////////////////
//                             Prototypes                             //
////////////////////////////////////////////////////////////////////////

void initGraph();
void initCriticalSpotsList();
void initStoppedConnectionsList();
int BFS(int, int);
int edmondsKarp(int, int);
void cleanMatrix();
void computeResults();
void returnOutput();

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                               PROGRAM                              //
//                                                                    //
////////////////////////////////////////////////////////////////////////

//////////////////////////////////Main//////////////////////////////////
//                					              //
//              Calls all functions that are needed to                //
//                        get the right result                        //
//                					              //
////////////////////////////////////////////////////////////////////////

int main(void){

  scanf("%d %d",&numSpots,&numConnections);

  initGraph();

  scanf("%d", &numProblems);
  cleanMatrix();

  for(int i=0; i<numProblems; i++){
      
    initCriticalSpotsList();
    initStoppedConnectionsList();
    computeResults();
    returnOutput();
  }
  
  delete graph;
  delete flowThroughSpotList;
  delete previousSpotsList;

  return 0;
}

/////////////////////////////InitGraph//////////////////////////////////
//                					              //
//             Initializes and fills structure graph                  //
//                					              //
////////////////////////////////////////////////////////////////////////

void initGraph(){

  int u, v;

  graph = new vector<vector<int>*> ();
  
  previousSpotsList = new int[numSpots];
  
  flowThroughSpotList = new vector<map<int, int>*>;

  for(int i=0; i<numSpots; i++){
    
    graph->push_back(new vector<int>());
    flowThroughSpotList->push_back(new map<int, int>());
  }

  for(int j=0; j<numConnections; j++){
    scanf("%d %d",&u ,&v);

    graph->at(u)->push_back(v);
    graph->at(v)->push_back(u);
  }
}

/////////////////////////InitCriticalSpotsList//////////////////////////
//                					              //
//    Initializes, reserves memory and fills Critical Spots List      //
//                					              //
////////////////////////////////////////////////////////////////////////

void initCriticalSpotsList(){

  int k;
  int temp;
  
  scanf("%d", &k); //k = numero de pontos envolvidos em cada lista de pontos críticos
  
  criticalSpotsList = new int[k]; 
  kInput = k;
  
  for(int j=0; j<k; j++){

    scanf("%d", &temp); //cada temp é um ponto crítico (uma lista tem vários pontos)
    criticalSpotsList[j] = temp; //cada posição de criticalSpotsList tem uma lista de pontos críticos
  }
}

//////////////////////////InitStoppedConnections////////////////////////
//                					              //
//                 Initializes Stopped Connections List               //
//                					              //
////////////////////////////////////////////////////////////////////////

void initStoppedConnectionsList(){

  numStoppedConnections = 0;
  stoppedConnections = infinito;
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                                 BFS                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////

int BFS(int source, int target){
 for(int h=0; h<numSpots; h++){
   previousSpotsList[h] = -1;
 }

 previousSpotsList[source] = -2;
 
 list<int> Queue;
 Queue.push_back(source);
 
 int currentSpot;
 
 while(!Queue.empty()){

  currentSpot = Queue.front(); 
  Queue.pop_front();

  for(vector<int>::iterator it = graph->at(currentSpot)->begin(); it != graph->at(currentSpot)->end(); it++){
   int actual = *it;

   if(previousSpotsList[actual] == -1){
    if(std::find(graph->at(currentSpot)->begin(), graph->at(currentSpot)->end(), actual)!=graph->at(currentSpot)->end() && flowThroughSpotList->at(currentSpot)->find(actual)->second == 0){
     previousSpotsList[actual] = currentSpot;
      
     if(actual!=target) {
      
       Queue.push_back(actual);
     } else return 1;
     
    }
   }
  }
 }
 
 return 0;
}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                             EDMONDS-KARP                           //
//                                                                    //
////////////////////////////////////////////////////////////////////////

int edmondsKarp(int source, int target){

  int maxFlow = 0;
  
  while(true){
    int flow = BFS(source, target);

    if(flow == 0) break;

    maxFlow += flow;
    int currentSpot = target;

    while(currentSpot != source){
    
      int previousSpot = previousSpotsList[currentSpot];
      flowThroughSpotList->at(previousSpot)->find(currentSpot)->second += flow;
      flowThroughSpotList->at(currentSpot)->find(previousSpot)->second -= flow;
      currentSpot=previousSpot;
    
    }
 }

 return maxFlow;
}

////////////////////////////CleanMatrix/////////////////////////////////
//                					              //
//              Resets all matrix values to default                   //
//                					              //
////////////////////////////////////////////////////////////////////////

void cleanMatrix(){
  for(int i=0; i<numSpots; i++){
    
    flowThroughSpotList->at(i)->clear();
  }
  
  for(int i=0; i<numSpots; i++){
    
    for (vector<int>::iterator it = graph->at(i)->begin(); it != graph->at(i)->end(); ++it){
      
      flowThroughSpotList->at(i)->insert(pair<int,int>(*it, 0));
      
    }
  }
}

////////////////////////////ComputeResults//////////////////////////////
//                					              //
//                        Executes algorithms                         //
//                   Fills structures with results                    //
//                					              //
////////////////////////////////////////////////////////////////////////

void computeResults(){
  
  numStoppedConnections = infinito;
    
  for(int j=0; j<kInput; j++){   
    
    for(int k=j+1; k<kInput; k++){
      
      int flow = edmondsKarp(criticalSpotsList[j], criticalSpotsList[k]);
      numStoppedConnections = min(numStoppedConnections, flow);
      cleanMatrix();
      
    }
  }
  
  stoppedConnections = numStoppedConnections;
  
  delete criticalSpotsList;
}

/////////////////////////////ReturnOutput///////////////////////////////
//                					              //
//                        Prints the results:                         //
//                        Stopped Connections		              //
//                					              //
////////////////////////////////////////////////////////////////////////

void returnOutput(){
  
  printf("%d\n", stoppedConnections);
}
