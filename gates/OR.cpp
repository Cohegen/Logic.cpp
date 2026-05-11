/*
Implementation of OR GATE.
It's false when both of its inputs are false
otherwise false
*/

#include <iostream>
using namespace std;

int main()
{
    int A,B;
    cout << "Enter input A (0 or 1): ";
    cin >> A;
    cout << "Enter input B (0 or 1): ";
    cin >> B;

    //defining logic function which produces output
    bool Y = (A || B);
    cout << "Y= " << Y;
    return 0;
}
