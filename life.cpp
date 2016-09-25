#include "life.h"
#include "userinput.h"
#include <istream>

using namespace std;

//------------------------------------------------------------------------------
LifeBoard::LifeBoard()
{
    /*
        This constructor initializes the board based on user input.
        LifeBoard::importMap() and LifeBoard::randomMap() functions create a
        2*width*height element 1D array at *board. This array contains both the
        current board and the future board. Essentially it is two 2D arrays
        that share a pointer alongside fancy indexing to make it all work.
        
        Indexing is caried out as follows:
        
        board [ (age%2)*width*height + vertPoss*width + horrPoss ]
        
        ((age%2)*width*height): selects for the current board. Use ((age+1)%2)
        to select the future board.
            ie. when creating a new board, place it in the other half of the
            array. That now becomes the current board and the old "current
            board" becomes the new future board. In this way, the two sides of
            the array flip between future and current.
        
        (vertPoss*width): skips rows equal to vertPoss.
        
        (horrPoss): move along the row to position horrPoss.
        
        Boundary conditions are also initialized in the constructor as a char
        variable boundaryCond with the rules:
        
        (boundaryCond=='1'): Classic boundary conditions
        (boundaryCond=='2'): Torroidal boundary conditions
        (boundaryCond=='3'): Mirror boundary conditions
    */
    cout << "Would you like to import a map (y)?: ";
    if (getCharInput()=='y')
    {
        importMap();
    }
    else
    {
        randomMap();
    }
    while (true)
    {
        cout << "What boundary conditions should be used? (select one)" << endl;
        cout << "(1): Classic boundary conditions" << endl;
        cout << "(2): Torroidal boundary conditions" <<endl;
        cout << "(3): Mirror boundary conditions" << endl;
        boundaryCond = getCharInput();
        if (boundaryCond!='1' and boundaryCond!='2' and boundaryCond!='3')
        {
            cout << "Must enter 1, 2, or 3." << endl;
        }
        else
        {
            break;
        }
    }
    age = 0;
    cout << "Would you like to save to a file? (y): ";
    if (getCharInput()=='y')
    {
        fileOutput = true;
        //then open the file stream
        cout << "Where should the data be saved?: ";
        cin >> filename;
        ifstream checkExists( filename.c_str() );
        while ( checkExists.good() )
        {
            cout << "This file already exists. Would you like to overwrite? (y): ";
            if ( getCharInput() == 'y')
            {
                break;
            }
            else
            {
                checkExists.close();
                cout << "Where should the data be saved?: ";
                cin >> filename;
                checkExists.open( filename.c_str() );
            }
        }
        ofs.open(filename.c_str());
    }
    else
    {
        fileOutput = false;
    }
}
//------------------------------------------------------------------------------
LifeBoard::~LifeBoard(void)
{
    /*
        The destructor frees up the memory alocated to the board and closes the
        output file stream
    */
    delete [] board;
    board = NULL;
    if (ofs.is_open())
    {
        ofs.close();
    }
}
//------------------------------------------------------------------------------
void LifeBoard::randomMap()
{
    /*
        This function generates a random starting board based on user input
        and stores it at *board
        Indexing rules are described in LifeBoard::LifeBoard()
    */
    cout << "Generating random map." << endl;
    cout << "What should the width be?: ";
    width = getIntInput();
    while (width>1000)
    {
        cout << "Error: maximum width (1000) exceeded. Please try again." << endl;
        cout << "What should the width be?: ";
        width = getIntInput();
    }
    cout << "What should the height be?: ";
    height = getIntInput();
    while (height>1000)
    {
        cout << "Error: maximum height (1000) exceeded. Please try again." << endl;
        cout << "What should the height be?: ";
        height = getIntInput();
    }
    cout << "What should the population density be?: ";
    float popDensity = getProbInput();
    board = new char [ 2*width*height ];
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            float normRandNum = rand()/(float)RAND_MAX;
            if (normRandNum<popDensity)
            {
                board[ width*height + i*width + j ] = '0'; //ie. blank future board
                board[ i*width + j ] = 'X';
            }
            else
            {
                board[ width*height + i*width + j ] = '0'; //ie. blank future board
                board[ i*width + j ] = '-';
            }
        }
    }
    cout << "A random map has been generated." << endl;
}
//------------------------------------------------------------------------------
void LifeBoard::importMap()
{
    /*
        This function imports a user map. Map must be of the following format:
        
        height
        width
        -X-----X--X--------
        ----X----X-------X-
        ---X-----------X---
        -------X-X---------
        ----X----------X---
        ---X---------------
        ------X--X-------X-
        
        where height and width match the height and width of the given board.
            Note: Currently there are no safeguards to ensure this is the case.
        
        The map is stored at *board using the indexing rules described
        in LifeBoard::LifeBoard()
    */
    ifstream ifs; //create input file stream
opening:
    cout << "Please enter the file path: " << endl;
    string filename;
    cin >> filename;
    ifs.open(filename.c_str());
    if (!ifs)
    {
        ifs.close();
        cout << "The file cannot be opened. Try again? (y): ";
        if (getCharInput()=='y')
        {
            goto opening;
        }
        else
        {
            randomMap();
        }
    }
    string heightString;
    string widthString;
    getline(ifs, heightString);
    getline(ifs, widthString);
    height = stringToInt(heightString);
    width = stringToInt(widthString);
    if ( (height == -1) or (width == -1) ) //-1 used as error indicator from stringToInt()
    {
        ifs.close();
        cout << "Error: Improper file format. First two lines must be height and width." << endl;
        cout << "Would you like to try a different file? (y): ";
        if ( getCharInput()=='y' )
        {
            goto opening;
        }
        else
        {
            randomMap();
        }
        return;
    }
    else if ( (height>1000) or (width>1000) )
    {
        ifs.close();
        cout << "Error: Improper file format. Max width and/or height (1000) exceeded." << endl;
        cout << "Would you like to try a different file? (y): ";
        if ( getCharInput()=='y' )
        {
            goto opening;
        }
        else
        {
            randomMap();
        }
        return;
    }
    board = new char [ 2*width*height ];
    string currLine; //stores the current line after reading it with getline
    for (int i=0; i<height; ++i)
    {
        getline(ifs, currLine);
        if ( currLine.length() != width )
        {
            delete board;
            cout << "Error: Improper file format. Given dimensions must match board." << endl;
            cout << "Would you like to try a different file? (y): ";
            if ( getCharInput()=='y' )
            {
                goto opening;
            }
            else
            {
                randomMap();
            }
            return;
        }
        for (int j=0; j<width; ++j)
        {
            if (currLine[j]=='X')
            {
                board[ width*height + i*width + j ] = '0'; //ie. blank future board
                board[ i*width + j ] = 'X';
            }
            else if (currLine[j]=='-')
            {
                board[ width*height + i*width + j ] = '0'; //ie. blank future board
                board[ i*width + j ] = '-';
            }
            else
            {
                delete board;
                cout << "Error: Improper file format. Improper character detected." << endl;
                cout << "Would you like to try a different file? (y): ";
                if ( getCharInput()=='y' )
                {
                    goto opening;
                }
                else
                {
                    randomMap();
                }
                return;
            }
        }
    }
    //checking to make sure we are at the end of the file (ie. dimensions match those given)
    if ( getline(ifs, currLine) )
    {
        delete board;
        cout << "Error: Improper file format. Given dimensions must match board. (excess lines)" << endl;
        cout << "Would you like to try a different file? (y): ";
        if ( getCharInput()=='y' )
        {
            goto opening;
        }
        else
        {
            randomMap();
        }
        return;
    }
    ifs.close();
}
//------------------------------------------------------------------------------
void LifeBoard::printBoard()
{
    /*
        This function prints out the generation number and the board stored at
        *board to standard output with proper line breaks
    */
    cout << age;
    for (int i=0; i<height; ++i)
    {
        cout << endl;
        for (int j=0; j<width; ++j)
        {
            cout << board[ (age%2)*width*height + i*width + j ];
        }
    }
    cout << endl;
}
//------------------------------------------------------------------------------
void LifeBoard::saveBoard()
{
    /*
        This function appends the current board to the output file stream ofs
        In order for this to function properly within this program, ofs should
        be initialized, opened, and closed in main()
        I may modify this to utilize the constructor and destructor for this
    */
    if (fileOutput == false)
    {
        //an error the user should never be able to see
        //if I make a coding mistake, this will alert me to it
        cout << "Error: saving board without open file." << endl;
    }
    ofs << age;
    for (int i=0; i<height; ++i)
    {
        ofs << endl;
        for (int j=0; j<width; ++j)
        {
            ofs << board[ (age%2)*width*height + i*width + j ];
        }
    }
    ofs << endl;
}
//------------------------------------------------------------------------------
unsigned int LifeBoard::countNeighborsClassic(int vertPoss, int horrPoss)
{
    /*
        This function counts the neighbors to a position on the board using
        classic rules where positions off the board are empty. This is done by
        creating a 2D array called "indices" that stores the indices of the
        appropriate neighbor along with a marker that can be either 1 or 0. If
        it is 1, the indices are used to check for a neighbor. If it is 0, then
        the position is off the board and is not checked. The function then
        returns the number of neighbors.
    */
    unsigned int theCount=0;
    int indices [8][3] = {
        { (vertPoss-1) , (horrPoss-1) , 1 } , //upper left    [0]
        { (vertPoss-1) , (horrPoss)   , 1 } , //upper center  [1]
        { (vertPoss-1) , (horrPoss+1) , 1 } , //upper right   [2]
        { (vertPoss)   , (horrPoss-1) , 1 } , //middle left   [3]
        { (vertPoss)   , (horrPoss+1) , 1 } , //middle right  [4]
        { (vertPoss+1) , (horrPoss-1) , 1 } , //lower left    [5]
        { (vertPoss+1) , (horrPoss)   , 1 } , //lower center  [6]
        { (vertPoss+1) , (horrPoss+1) , 1 } , //lower right   [7]
    };
    //check if at top edge of board
    if (vertPoss==0)
    {
        //ignore upper neighbors
        indices[0][2] = 0;
        indices[1][2] = 0;
        indices[2][2] = 0;
    }
    //check if at left edge of board
    if (horrPoss==0)
    {
        //ignore left neighbors
        indices[0][2] = 0;
        indices[3][2] = 0;
        indices[5][2] = 0;
    }
    //check if at bottom edge of board
    if (vertPoss==height-1)
    {
        //ignore lower neighbors
        indices[5][2] = 0;
        indices[6][2] = 0;
        indices[7][2] = 0;
    }
    //check if at right edge of board
    if (horrPoss==width-1)
    {
        //ignore right neighbors
        indices[2][2] = 0;
        indices[4][2] = 0;
        indices[7][2] = 0;
    }
    //count neighbors in a loop over the indices
    for (int i=0; i<8; ++i)
    {
        if (indices[i][2]==1)
        {
            if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='X')
            {
                theCount++;
            }
            else if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='-')
            {
                //do nothing
            }
            else
            {
                cout << "Error: counting off the board." << endl;
            }
        }
    }
    return theCount;
}
//------------------------------------------------------------------------------
unsigned int LifeBoard::countNeighborsTorus(int vertPoss, int horrPoss)
{
    /*
        This function counts the neighbors to a position on the board using
        torroidal ("doughnut") rules where positions off the board are wraped
        around like a torus. This is done by creating a 2D array called
        "indices" that stores the indices of the appropriate neighbor. The
        function checks if the current position is at a boundary and reassigns
        the indices to those on the other edge if appropriate.
        The function then returns the number of neighbors.
    */
    unsigned int theCount=0;
    int indices [8][2] = {
        { (vertPoss-1) , (horrPoss-1) } , //upper left    [0]
        { (vertPoss-1) , (horrPoss)   } , //upper center  [1]
        { (vertPoss-1) , (horrPoss+1) } , //upper right   [2]
        { (vertPoss)   , (horrPoss-1) } , //middle left   [3]
        { (vertPoss)   , (horrPoss+1) } , //middle right  [4]
        { (vertPoss+1) , (horrPoss-1) } , //lower left    [5]
        { (vertPoss+1) , (horrPoss)   } , //lower center  [6]
        { (vertPoss+1) , (horrPoss+1) } , //lower right   [7]
    };
    //check if at top edge of board
    if (vertPoss==0)
    {
        //wrap indices around to the bottom
        indices[0][0] = height-1;
        indices[1][0] = height-1;
        indices[2][0] = height-1;
    }
    //check if at left edge of board
    if (horrPoss==0)
    {
        //wrap indices around to the right
        indices[0][1] = width-1;
        indices[3][1] = width-1;
        indices[5][1] = width-1;
    }
    //check if at bottom edge of board
    if (vertPoss==height-1)
    {
        //wrap indices around to the top
        indices[5][0] = 0;
        indices[6][0] = 0;
        indices[7][0] = 0;
    }
    //check if at right edge of board
    if (horrPoss==width-1)
    {
        //wrap indices around to the left
        indices[2][1] = 0;
        indices[4][1] = 0;
        indices[7][1] = 0;
    }
    //count neighbors in loop over indices
    for (int i=0; i<8; ++i)
    {
        if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='X')
        {
            theCount++;
        }
        else if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='-')
        {
            //do nothing
        }
        else
        {
            cout << "Error: counting off the board." << endl;
        }
    }
    return theCount;
}
//------------------------------------------------------------------------------
unsigned int LifeBoard::countNeighborsMirror(int vertPoss, int horrPoss)
{
    /*
        This function counts the neighbors to a position on the board using
        mirror rules where positions off the board are mirror images of
        positions on the other edge of the boundary. This is done by creating a
        2D array called "indices" that stores the indices of the appropriate
        neighbor. The function checks if the current position is at a boundary
        and reassigns the indices to those mirrored by the boundary if
        appropriate. The function then returns the number of neighbors.
    */
    unsigned int theCount=0;
    int indices [8][2] = {
        { (vertPoss-1) , (horrPoss-1) } , //upper left    [0]
        { (vertPoss-1) , (horrPoss)   } , //upper center  [1]
        { (vertPoss-1) , (horrPoss+1) } , //upper right   [2]
        { (vertPoss)   , (horrPoss-1) } , //middle left   [3]
        { (vertPoss)   , (horrPoss+1) } , //middle right  [4]
        { (vertPoss+1) , (horrPoss-1) } , //lower left    [5]
        { (vertPoss+1) , (horrPoss)   } , //lower center  [6]
        { (vertPoss+1) , (horrPoss+1) } , //lower right   [7]
    };
    //check if at top edge of board
    if (vertPoss==0)
    {
        //mirror indices for top
        indices[0][0] = 0;
        indices[1][0] = 0;
        indices[2][0] = 0;
    }
    //check if at left edge of board
    if (horrPoss==0)
    {
        //mirror indices for left
        indices[0][1] = 0;
        indices[3][1] = 0;
        indices[5][1] = 0;
    }
    //check if at bottom edge of board
    if (vertPoss==height-1)
    {
        //mirror indices for bottom
        indices[5][0] = height-1;
        indices[6][0] = height-1;
        indices[7][0] = height-1;
    }
    //check if at right edge of board
    if (horrPoss==width-1)
    {
        //mirror indices for right
        indices[2][1] = width-1;
        indices[4][1] = width-1;
        indices[7][1] = width-1;
    }
    //count neighbors in loop over indices
    for (int i=0; i<8; ++i)
    {
        if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='X')
        {
            theCount++;
        }
        else if (board[ (age%2)*width*height + indices[i][0]*width + indices[i][1] ]=='-')
        {
            //do nothing
        }
        else
        {
            cout << "Error: counting off the board." << endl;
        }
    }
    return theCount;
}
//------------------------------------------------------------------------------
bool LifeBoard::aliveInFuture(unsigned int neighbors, bool aliveNow)
{
    /*
        This function takes the number of neighbors and whether a cell is
        occupied and returns a bool correspoinding to whether the cell should
        be occupied in the future board
    */
    if (neighbors==2)
    {
        return aliveNow;
    }
    else if (neighbors==3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
bool LifeBoard::isStatic()
{
    //loop through all points
    //if matches: do nothing
    //else: return false
    //after loop is over, return true
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            if ( board[ width*height + i*width + j ] != board[ i*width + j ] )
            {
                return false;
            }
        }
    }
    return true;
}



//------------------------------------------------------------------------------
void LifeBoard::nextBoard()
{
    /*
        This function combines many of the other functions in the class to
        incriment the board by one generation. In doing so, the age counter is
        incrimented. The board must already exist at *board before this function
        is called, however that should be handled by the constructor.
        Indexing rules for board are described in LifeBoard::LifeBoard()
    */
    //for (each position [i,j] in the board)
    for (int i=0; i<height; ++i)
    {
        for (int j=0; j<width; ++j)
        {
            //initialize some storage variables
            bool aliveNow = false;
            int neighbors = 0;
            //use the appropriate counting function
            if (boundaryCond=='1')
            {
                neighbors = countNeighborsClassic(i,j);
            }
            else if (boundaryCond=='2')
            {
                neighbors = countNeighborsTorus(i,j);
            }
            else if (boundaryCond=='3')
            {
                neighbors = countNeighborsMirror(i,j);
            }
            else
            {
                cout << "Error: boundary conditions not specified." << endl;
            }
            //check to see if the position is currently alive
            if ( board[ (age%2)*width*height + i*width + j ] == 'X')
            {
                aliveNow = true;
            }
            //mark with an 'X' in the future board if appropriate
            if (aliveInFuture(neighbors, aliveNow))
            {
                board[ ((age+1)%2)*width*height + i*width + j ] = 'X';
            }
            //otherwise mark with a '-'
            else
            {
                board[ ((age+1)%2)*width*height + i*width + j ] = '-';
            }
        }
    }
    age++; //track iterations for output and index flipping
}