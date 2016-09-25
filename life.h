#include <iostream>
#include <cstdlib>
#include <fstream>
#include "userinput.h"

using namespace std;

class LifeBoard
{
    public:
        string filename;
        ofstream ofs;
        unsigned int age;
        char boundaryCond;
        unsigned int width;
        unsigned int height;
        bool fileOutput;
        LifeBoard();
        ~LifeBoard();
        void randomMap();
        void importMap();
        void printBoard();
        void saveBoard();
        void nextBoard();
        char outputType;
        char* board;
        unsigned int countNeighborsClassic(int vertPoss, int horrPoss);
        unsigned int countNeighborsTorus(int vertPoss, int horrPoss);
        unsigned int countNeighborsMirror(int vertPoss, int horrPoss);
        bool aliveInFuture(unsigned int neighbors, bool aliveNow);
        bool isStatic();
};