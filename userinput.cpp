#include <iostream>
#include <math.h>
#include "userinput.h"

using namespace std;

char getCharInput()
{
    /*
        This function asks the user for a char and returns it
    */
    string userInput;
    while (true)
    {
        cin >> userInput;
        if (userInput.length() == 1)
        {
            return userInput[0];
        }
        else
        {
            cout << "Must enter a single character." << endl;
        }
    }
}

int getIntInput()
{
    string userInput;
    cin >> userInput;
    int integer = 0;
    int digit;
    for (int i=1; i<=userInput.length(); ++i)
    {
        if (userInput[userInput.length()-i]=='0')
        {
            digit = 0;
        }
        else if (userInput[userInput.length()-i]=='1')
        {
            digit = 1;
        }
        else if (userInput[userInput.length()-i]=='2')
        {
            digit = 2;
        }
        else if (userInput[userInput.length()-i]=='3')
        {
            digit = 3;
        }
        else if (userInput[userInput.length()-i]=='4')
        {
            digit = 4;
        }
        else if (userInput[userInput.length()-i]=='5')
        {
            digit = 5;
        }
        else if (userInput[userInput.length()-i]=='6')
        {
            digit = 6;
        }
        else if (userInput[userInput.length()-i]=='7')
        {
            digit = 7;
        }
        else if (userInput[userInput.length()-i]=='8')
        {
            digit = 8;
        }
        else if (userInput[userInput.length()-i]=='9')
        {
            digit = 9;
        }
        else
        {
            cout << "Error: Not an integer. return 1" << endl;
            return 1;
        }
        integer = integer + digit*pow(10, (i-1));
    }
    return integer;
}

int stringToInt(string userInput)
{
    int integer = 0;
    int digit;
    for (int i=1; i<=userInput.length(); ++i)
    {
        if (userInput[userInput.length()-i]=='0')
        {
            digit = 0;
        }
        else if (userInput[userInput.length()-i]=='1')
        {
            digit = 1;
        }
        else if (userInput[userInput.length()-i]=='2')
        {
            digit = 2;
        }
        else if (userInput[userInput.length()-i]=='3')
        {
            digit = 3;
        }
        else if (userInput[userInput.length()-i]=='4')
        {
            digit = 4;
        }
        else if (userInput[userInput.length()-i]=='5')
        {
            digit = 5;
        }
        else if (userInput[userInput.length()-i]=='6')
        {
            digit = 6;
        }
        else if (userInput[userInput.length()-i]=='7')
        {
            digit = 7;
        }
        else if (userInput[userInput.length()-i]=='8')
        {
            digit = 8;
        }
        else if (userInput[userInput.length()-i]=='9')
        {
            digit = 9;
        }
        else
        {
            cout << "Error: Not an integer. return -1" << endl;
            return -1;
        }
        integer = integer + digit*pow(10, (i-1));
    }
    return integer;
}





float getProbInput()
{
    float userInput;
    while (true)
    {
        if (cin >> userInput)
        {
            if (userInput<=1 and userInput>=0)
            {
                return userInput;
            }
            else
            {
                cout << "Error: Must be between zero and one. Try again." << endl;
            }
        }
        else
        {
            cout << "Error: Must be a number. Try again." << endl;
        }
    }
}
