#ifndef CLASS_GRAPH_HH
#define CLASS_GRAPH_HH

// INCLUDES
#include <iostream>
#include <random>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <omp.h>
using namespace std;

class graph {

private:
  vector<vector<int>> ConjVert; // Vertexs and adjacent vertexs
  vector < int > props;
  vector < pair < int,int > > cycle;
  vector < bool > matched;
  int nvert; // number of vertex


public:
  // Constructors
  graph();
  graph(int nv);

  // Modifiers


  vector<int> permutacion(int n);
  void buildinput();

  vector<vector<int>> getConjVert() {return ConjVert;}
  
  bool some_empty();
  bool all_size_1();

  int position(int i, int j);
  int getUnmatched();
  int getInitial();
  bool is_cycle(pair<int,int> last);
  void isolate(int i, int unique);

  void reject_simetrical(int a, int b);
  void reject(int b, int a);
  
  bool step1();
  void step2();
  bool step3();

  void printConjVert();
  void printProps();


  // Consults



};
#endif
