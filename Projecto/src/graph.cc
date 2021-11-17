#include "graph.hh"
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

bool debug = false;

// Constructors
graph::graph() {
  //ConjVert = vector<vector<int>> ();
  nvert=0;
}

graph::graph(int nv) {
  nvert = nv;
  ConjVert = vector<vector<int>> (nvert*2);
  pair < int,int > init = {-1, -1};
  props = vector < int > (nvert*2, -1);
  matched = vector < bool > (nvert*2, false);
  buildinput();
  
  /*ConjVert = {  {1, 3, 5, 2, 4},
                {3, 4, 5, 0, 2},
                {3, 4, 5, 0, 1},
                {5, 2, 0, 4, 1},
                {5, 2, 3, 1, 0},
                {0, 1, 3, 2, 4} };*/
}

void graph::buildinput() {
  //list<int> l;
  for (int i = 0; i < nvert*2; ++i) {
    //srand(int (time(0)));
    vector<int> aux = permutacion(i);
    ConjVert[i] = aux;
  }
}


vector<int> graph::permutacion(int n) {
    vector<int> l (0);
    for (int i=0; i<nvert*2; ++i) {
      if (i != n) l.push_back(i);
    }
    vector <int> aux (nvert*2 - 1);
    for (int i = 0; i < nvert*2 - 1; ++i) {

      int index = rand() % (l.size());
      int valor = l[index];
      l.erase(l.begin()+index);
      aux[i] = valor;

    }
    return aux;
}

void graph::printProps () {
    for (int i = 0; i < props.size(); ++i) {
        cout << props[i] << ", ";
    }
    cout << endl;
}

void graph::printConjVert() {
  for (int i = 0; i < ConjVert.size(); ++i) {
    cout << i << ",";
    for (int j = 0; j < ConjVert[i].size(); ++j) {
      cout << ConjVert[i][j] << ",";
    }
    cout << endl;
  }
}

int graph::getUnmatched() {
  for (int i = 0; i < matched.size(); ++i) {
    if (!matched[i]) return i;
  }
  return -1;
}

void graph::reject_simetrical(int a, int b) {
  reject(a, b);
  reject(b, a);
}

void graph::reject(int a, int b) {
  int pos = position(a, b);
  if (pos >= 0) ConjVert[a].erase(ConjVert[a].begin()+pos);
}

int graph::position(int i, int j) {
  for (int k = 0; k < ConjVert[i].size(); ++k) {
    if (ConjVert[i][k] == j) return k;
  }
  return -1;
}

bool graph::some_empty() {
  for (int i = 0; i < ConjVert.size(); ++i) {
    if (ConjVert[i].size() == 0) return true;
  }
  return false;
}

bool graph::all_size_1() {
  for (int i = 0; i < ConjVert.size(); ++i) {
    if (ConjVert[i].size() != 1) return false;
  }
  return true;
}

int graph::getInitial() {
  for (int i = 0; i < ConjVert.size(); ++i) {
    if (ConjVert[i].size() > 1) return i;
  }
  return -1; 
}

bool graph::step1() {
  int unmatched = 0; 
  while (unmatched >= 0 && !some_empty()) {

    int first = ConjVert[unmatched][0];
    int pos = position(first, unmatched);

    if (props[first] == -1) {
      props[first] = unmatched;
      matched[unmatched] = true;

    }
    else if (position (first, props[first]) > pos) {
      int exProp = props[first];
      //reject
      reject_simetrical(exProp, first);
      matched[exProp] = false;
      //accept
      props[first] = unmatched;
      matched[unmatched] = true;
    }
    else {
      //reject
      reject_simetrical(unmatched, first);
    }
    unmatched = getUnmatched();
  }
  return !some_empty();
}

void graph::step2() {
  for (int i = 0; i < ConjVert.size(); ++i) {
    int pos = position( i, props[i]) + 1;
    while (pos < ConjVert[i].size()) reject_simetrical(i, ConjVert[i][pos]);
  }
}

bool graph::step3() {
  while (!some_empty() && !all_size_1()) {
    int initial = getInitial();
    cycle = vector <pair < int,int > > (0);
    int second = ConjVert[initial][1];
    int last = ConjVert[second][ConjVert[second].size() - 1];
    cycle.push_back({second, last});
    bool found = false;
    while (!found) {
      second = ConjVert[last][1];
      last = ConjVert[second][ConjVert[second].size() - 1];
      if (is_cycle({second, last})) found = true;
      cycle.push_back({second, last});
    }
    for (int i = 0; i < cycle.size(); ++i) {
      reject_simetrical(cycle[i].first, cycle[i].second);
    }
    for (int i = 0; i < ConjVert.size(); ++i) {
      if (ConjVert[i].size() == 1) isolate( ConjVert[i][0], i);
    }
  }
  return all_size_1();
  
}

bool graph::is_cycle(pair<int,int> lastPair) {
  bool found = false;
  int i = 0;
  while ( i < cycle.size() && !found) {
    if (cycle[i] == lastPair) found = true; 
    else ++i;
  }
  if (found) {
    cycle.erase(cycle.begin(), cycle.begin()+i);
    return true;
  }
  else return false;
}

void graph::isolate(int k, int unique) {
  for (int i = 0; i < ConjVert[k].size(); ++i) {
    int to_delete = ConjVert[k][i];
    if (to_delete != unique) {
      reject_simetrical(k , to_delete);
      if (ConjVert[to_delete].size() == 1) isolate(ConjVert[to_delete][0], to_delete);
    }
  }
}

/*
0  --  3, 7, 6, 4, 8
1  --  2, 9
2  --  1, 6, 4
3  --  0, 5
4  --  6, 8, 0, 2
5  --  8, 3
6  --  4, 0, 8, 2
7  --  9, 0, 8, 3
8  --  5, 0, 6, 3, 7, 9, 4
9  --  7, 8, 1
*/