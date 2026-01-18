// fig02_05.cpp
// Comparing integers using if statements, relational operators
// and equality operators.
#include <iostream> // enables program to perform input and output

using std::cout; // program uses cout
using std::cin; // program uses cin

// function main begins program execution
int main() {
   int number1{0}; // first integer to compare (initialized to 0)
   int number2{0}; // second integer to compare (initialized to 0)
   
   cout << "Enter two integers to compare: "; // prompt user for data
   cin >> number1 >> number2; // read two integers from user

   if (number1 == number2) {
      cout << number1 << " == " << number2 << "\n";
   }

   if (number1 != number2) {
      cout << number1 << " != " << number2 << "\n";
   }

   if (number1 < number2) {
      cout << number1 << " < " << number2 << "\n";
   }

   if (number1 > number2) {
      cout << number1 << " > " << number2 << "\n";
   }

   if (number1 <= number2) {
      cout << number1 << " <= " << number2 << "\n";
   }

   if (number1 >= number2) {
      cout << number1 << " >= " << number2 << "\n";
   }
} // end function main
