/*
NAND gate is the inverse of AND gate
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

    if((A ==true )&& (B == true))
    {
        cout << "Y= " << false;
    }
    else{
        cout << "Y= " << true;
    }

    return 0;
}
