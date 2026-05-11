/*
This program implements the NOT Gate.
The NOT gate has one input A and one output Y
*/

#include <iostream>
using namespace std;


int main()
{
    //defining input
    bool A = true;

    //output
    bool Y = !A;

    cout << "A= " <<A<<endl;
    cout << "NOT A="<< Y;

    return 0;
}
