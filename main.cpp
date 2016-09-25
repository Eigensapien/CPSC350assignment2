#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "life.h"
#include "userinput.h"

using namespace std;

int main()
{
    LifeBoard board;        //Create the board
    char pause;
    while (true)
    {
        cout << "Would you like to pause? (select one)" << endl;
        cout << "(1): Wait one second between frames" << endl;
        cout << "(2): Wait for user input between frames" <<endl;
        cout << "(3): No waiting between frames" << endl;
        pause = getCharInput();
        if (pause!='1' and pause!='2' and pause!='3')
        {
            cout << "Must enter 1, 2, or 3." << endl;
        }
        else
        {
            break;
        }
    }
    while ( !board.isStatic() )
    {
        //Wait if desired
        if (pause=='1')
        {
            //Unfortunately, there is no universal sleep function,
            //so this only works for UNIX machines.
            usleep(1000000); //sleep for 1 second
        }
        else if (pause=='2')
        {
            //using cin to wait until user presses enter
            cin.ignore();
        }
        //Output to terminal
        board.printBoard();
        //Output to file
        if ( board.fileOutput )
        {
            board.saveBoard();
        }
        //Move to the next frame
        board.nextBoard();
        if ( (board.age%10000) == 0)
        {
            // this condition activates every 10,000 generations and makes
            // sure that the user would like to keep going. This gives the
            // user a means to exit from infinite loops caused by oscillators
            cout << "This is the " << board.age << "th generation." << endl;
            cout << "Would you like to continue? (y): ";
            if ( getCharInput() != 'y' )
            {
                break;
            }
        }
    }
    cout << "Static condition reached. Press enter to exit.";
    cin.ignore();
    cin.get();
}