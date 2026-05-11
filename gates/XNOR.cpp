/*
Implementation of an XNOR gate
It is true when both inputs have similar values
*/

#include <iostream>
using namespace std;

int main()
{
    int A,B;
    cout << "Enter input A(0 or 1)";
    cin >>A;

    cout <<"Enter input B (0 or 1)";
    cin >> B;

    if(A==B)
    {
        cout << "Y= " << true;
    }
    else{
        cout << "Y= " << false;
    }

    return 0;
}
