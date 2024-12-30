#include<iostream>
#include<string>
#include "symls.hpp"
using namespace std;
int main()
{
    string input;
    cout<<"Enter no of watermelons: ";
    cin >> input;
    string output = "";
    string code =
        "let integer w and read w\n"
        "if w modulo 2 is equal-to 0 then\n"
        "if w is greater-than 2 then\n"
        "print 'YES' else\n"
        "print 'NO' stop else\n"
        "print 'NO' stop";

    symboless::Symboless s(code, input, output);

    cout << output;
}