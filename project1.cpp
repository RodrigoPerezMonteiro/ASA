#include <stdio.h>
#include <string.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <list>
#include <iostream>
#include <iterator>
#include <climits>
using namespace std;

////////////////////////////////////////////////////////////////////////
//                           al061                                    //
//                    73138 António Pólvora                           //
//                    73701 Rodrigo Monteiro                          //
////////////////////////////////////////////////////////////////////////
#define infinito INT_MAX

/////////////////////////////Structures/////////////////////////////////
vector<list<int>* > personRelations;
vector<int> dNodes;
vector<int> lowNodes;
stack<int> stck; 
vector<bool> stckAux;              
vector< list<int> > isolatedNodes; 
////////////////////////////////////////////////////////////////////////

/////////////////////////////Global Variables///////////////////////////
int numVisitedNodes = 0;
int numConections = 0;
int numPersons = 0;
int cycles = 0;
int maxNodes = 0;
int isolatedSCC = 0;
////////////////////////////////////////////////////////////////////////

/////////////////////////////Prototypes/////////////////////////////////
void tarjanInit();
void tarjanVisit(int u);
void calcIsolated();
void receiveInitialInfo();
void initStructures();
void buildGraph();
void returnOutPut();
////////////////////////////////////////////////////////////////////////


/////////////////////////////Main Function//////////////////////////////
//              Calls all functions that are needed to                //
//              get the right result                                  //
////////////////////////////////////////////////////////////////////////
int main(void){ 
    
    receiveInitialInfo();
    initStructures();
    buildGraph();
    tarjanInit();
    calcIsolated();
    returnOutPut();    

    return 0;
}

/////////////////////////////ReceiveInitialInfo/////////////////////////
//              Receive from input the number of nodes and            //
//              the number of conections                              //
////////////////////////////////////////////////////////////////////////
void receiveInitialInfo(){

    scanf("%d %d",&numPersons,&numConections);

}


/////////////////////////////InitStructures/////////////////////////////
//              Initializes all the structures that keep info         //
////////////////////////////////////////////////////////////////////////
void initStructures(){

    dNodes.reserve(numPersons);
    lowNodes.reserve(numPersons);
    stckAux.reserve(numPersons);
    personRelations.reserve(numPersons);

}

/////////////////////////////BuildGraph/////////////////////////////////
//             Insert information from output to our structures       //
//             Relates people with their connections                  //
////////////////////////////////////////////////////////////////////////
void buildGraph(){

    int u, v;

    for(int h=0; h<numPersons; h++){
      personRelations[h] = new list<int>();
    }
    
    for(int i=0; i <numConections; i++){
      scanf("%d %d",&u ,&v);
      //adicionar pessoas e conexoes na personRelations
      personRelations[u-1]->push_back(v-1);
    }

}

/////////////////////////////ReturnOutPut///////////////////////////////
//                    Prints the results                              //
//                       - Number of SCC'S                            //
//                       - Number of nodes of the biggest SCC         //
//                       - Number of SCC'S with only one node         //
////////////////////////////////////////////////////////////////////////
void returnOutPut(){

  printf("%d\n%d\n%d\n", cycles, maxNodes, isolatedSCC);

}


/////////////////////////////CalcIsolated/////////////////////////////////
//             Calculates the number of SCC'S with only one node        //
//////////////////////////////////////////////////////////////////////////
void calcIsolated(){
  int isolated = 0;

  for (vector< list<int> >::iterator Iterator1 = isolatedNodes.begin(); Iterator1 != isolatedNodes.end(); ++Iterator1) {
    list<int> components = *Iterator1;
    
    bool isolatedNode = true;
    for (list<int>::iterator Iterator2 = components.begin(); Iterator2 != components.end(); ++Iterator2) {
      int temp = *Iterator2;
      
      list<int>* adjacentToTemp = personRelations[temp];
      
      for (list<int>::iterator Iterator3 = adjacentToTemp->begin(); Iterator3 != adjacentToTemp->end(); ++Iterator3) {
	int adjacentNode = *Iterator3;
	
	if(lowNodes[adjacentNode] != lowNodes[temp]){
	  isolatedNode = false;
	  bool noCyclesWithDifferentLow = false;
	  for (list<int>::iterator Iterator4 = components.begin(); Iterator4 != components.end(); ++Iterator4) {
	    if(*Iterator4 == adjacentNode){
		  noCyclesWithDifferentLow = true;
	    }
	  }
	  
	  if(noCyclesWithDifferentLow) isolatedNode = true;
	}
      }
    }
    
    if(isolatedNode) isolated++;
  }
    isolatedSCC = isolated;
}

/////////////////////////////CalcIsolated/////////////////////////////////
//             Calculates the number of SCC'S with only one node        //
//////////////////////////////////////////////////////////////////////////
void tarjanInit(){
    numVisitedNodes = 0;

    for (int i = 0; i < numPersons; i++){
      stckAux[i] = false;
      dNodes[i] = infinito;	
      lowNodes[i] = infinito;
    }
    
    for (int i = 0; i < numPersons; i++){
      if (dNodes[i] == infinito){
      tarjanVisit(i);
      }
    }
}


/////////////////////////////CalcIsolated/////////////////////////////////
//             Apply the Tarjan algorithm                               //
//////////////////////////////////////////////////////////////////////////
void tarjanVisit(int u){
    
    dNodes[u] = lowNodes[u] = numVisitedNodes;
    numVisitedNodes++;
    
    stck.push(u);
    stckAux[u] = true;  
    
    int nNodes = 0;  
    int v;
    
    list<int> isolated;
    list<int>* neighbours = personRelations[u];
    
    for (list<int>::iterator Iterator = neighbours->begin(); Iterator != neighbours->end(); ++Iterator) {
      int i = *Iterator; 

      if (dNodes[i] == infinito || stckAux[i] == true){
	  //Ignora vértices de SCCs já identiﬁcados
	  
	       if (dNodes[i] == infinito){
	           tarjanVisit(i);
	       }
	  
	     lowNodes[u] = min(lowNodes[u], lowNodes[i]);
      } 
    }

    if(dNodes[u] == lowNodes[u]){ //then repeat v = Pop(L) until u = v	
      
      do{
	  v = stck.top();
	  isolated.push_front(v);
	  stck.pop();
	  stckAux[v] = false;
	  nNodes++;
	} while (u!=v);
    
      cycles++;
      maxNodes = max(nNodes, maxNodes);
      isolatedNodes.push_back(isolated);

    }
}
