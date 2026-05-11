/*
Implementation of XOR
It is also known as XOR,it is true if and only if
its inputs have different values otherwise false
*/

#include <iostream>
using namespace std;

int main()
{
    int A,B;
    cout << "Enter input A: ";
    cin >> A;

    cout << "Enter input B: ";
    cin >> B;

    //defining the logic behind XOR
    if(A!=B)
    {
        cout << "Y= " << true;
    }
    else{
        cout << "Y= " << false;
    }

    return 0;
}
