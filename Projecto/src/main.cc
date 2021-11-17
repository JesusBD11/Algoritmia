#include <iostream>
#include <random>
#include <omp.h>
#include <stdlib.h>
#include "graph.hh"
#include <ctime>
#include "pbPlots.hpp"
#include "supportLib.hpp"

using namespace std;

void debuger (int n) 
{
  graph g(n);
  g.printConjVert();
  bool teSol = false;

  cout << endl;

  cout << "start 1" << endl;
  teSol = g.step1();
  cout << "finish 1" << endl;
  cout << endl;
  g.printConjVert();
  cout << endl;
  g.printProps();
  cout << endl;

  if (!teSol) cout << "no te solucio" << endl;

  else {
    cout << "start 2" << endl;
    g.step2();
    cout << "finish 2" << endl;
    cout << endl;
    g.printConjVert();
    cout << endl;
    g.printProps();
    cout << endl;

    cout << "start 3" << endl;
    teSol = g.step3();
    cout << "finish 3" << endl;
    cout << endl;
    g.printConjVert();
    cout << endl;
    g.printProps();
    cout << endl;

    if (teSol) cout << "te solucio" << endl;
    else cout << "no te solucio" << endl;
  }
}

void getPlot (int amplada, int alcada, vector<double> nProb, vector<double> probabs, string pngName ) {

  RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
  DrawScatterPlot(imageRef, 600, 400, &nProb, &probabs);
  vector<double> *pngData = ConvertToPNG(imageRef->image);
  WriteToFile(pngData, pngName);
  DeleteImage(imageRef->image);

}


int main() {

  int program;
  srand(int (time(0)));
  cout << "--------------------- Programa para estudiar Stable Rommates Problem ---------------------" << endl;

  cout << endl;
  cout << "  1 - Generar probabilidad de Martens completa." << endl;
  cout << "  2 - Generar plots para fallo en la fase 1 y plot para fallo en la fase 3." << endl;
  cout << "  3 - Generar probabilidad a partir de una n y numero de loops." << endl;
  cout << endl;
  cout << "Introduzca el numero del programa: ";
  cin >> program;
  cout << endl;

  vector<double> probabs (0);
  vector<double> nProb (0);
  vector<double> rejectFirst (0);
  vector<double> rejectLast (0);

  if (program == 1) {
      
      int loops = 1000;
      bool setLoop = true;
      
      int nMax = 0;
      cout << "Insertar la n maxima: ";
      cin >> nMax;
      cout << endl;

      if (setLoop) {
        cout << "Insertar el numero de loops para cada n: ";
        cin >> loops;
        cout << endl;
      }

      int n = 1;
      while (n <= nMax) {
        cout << n << endl;
        int counter = 0;
        for (int i = 0; i < loops; ++i) {
          graph g(n);
          bool teSol = false;
          teSol = g.step1();

          if (teSol) {
            g.step2();
            teSol = g.step3();
            if (teSol) ++counter;
          }
          
        }
        double probability = double (counter)/double (loops);
        probabs.push_back(probability);
        nProb.push_back(double (n));
        cout << probability << endl;
        if (n < 100) ++n;
        else if (n < 250) n+=5;
        else n+=10;
      }
      getPlot(720, 512, nProb, probabs, "ProbMartensPlot.png");

     
  }
  else if (program == 2) {

      int loops = 100;
      bool setLoop = true;

      int nMax = 0;
      cout << "Insertar la n maxima: ";
      cin >> nMax;
      cout << endl;

      if (setLoop) {
        cout << "Insertar el numero de loops para cada n: ";
        cin >> loops;
        cout << endl;
      }

      int n = 1;
      while (n <= nMax) {
        cout << n << endl;
        int countDeniedFirst = 0;
        int countDeniedLast = 0;
        for (int i = 0; i < loops; ++i) {
          graph g(n);
          bool teSol = false;
          teSol = g.step1();
          if (!teSol) ++countDeniedFirst;
          else {
            g.step2();
            teSol = g.step3();
            if (!teSol) ++countDeniedLast;
          }
          
        }
        rejectFirst.push_back(double (countDeniedFirst)/double (loops));
        rejectLast.push_back(double (countDeniedLast)/double (loops));
        nProb.push_back(double (n));

        if (n < 100) ++n;
        else if (n < 250) n+=5;
        else n+=10;
      }

      getPlot(720, 512, nProb, rejectFirst, "ProbRejectFirstPlot.png");
      getPlot(720, 512, nProb, rejectLast, "ProbRejectLastPlot.png");
  }
  else if (program == 3) {
      int n, loops;
      cout << "Insertar la n: ";
      cin >> n;
      cout << endl;

      cout << "Insertar el numero de loops para la n:: ";
      cin >> loops;
      cout << endl;

      string debug;
      cout << "Modo Debug, si o no? ";
      cin >> debug;
      int counter = 0;
      for (int i = 1; i < loops; ++i) {
        graph g(n);
        if (debug == "si") g.printConjVert();
        if (debug == "si") cout << endl;
        bool teSol = false;
        teSol = g.step1();
        if (teSol) {
          g.step2();
          if (debug == "si") g.printConjVert();
          if (debug == "si") cout << endl;
          teSol = g.step3();
          if (teSol) ++counter;
        }
        if (debug == "si") g.printConjVert();
        if (debug == "si") cout << endl;
      }
      cout << "Probabilidad de ser estable con n = " << n << ": " << double (counter)/double (loops) << endl;
  }
}
