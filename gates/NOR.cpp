/*
Implementation of NOR gate
NOR gate is the inverse of OR gate
*/

#include <iostream>
using namespace std;

int main()
{
    int A,B;
    cout << "Enter input A: ";
    cin >>A;

    cout << "Enter input B: ";
    cin >> B;

    if((A==false) && (B==false))
    {
        cout << "Y= " << true;
    }
    else{
        cout << "Y= " << false;
    }
    return 0;
}
