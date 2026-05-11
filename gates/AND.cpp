/*
Implementation of AND gate
In AND gate, the result is true only if
both inputs are TRUE
*/
#include <iostream>
using namespace std;

int main()
{
    int A,B;
    cout << "Enter input A(0 or 1): ";
    cin >> A;

    cout << "Enter input B(0 or 1): ";
    cin >> B;

    if(A==true && B==true)
    {
        cout << true;
    }
    else{
        cout << false;
    }

    return 0;
}
