// fig04_07.cpp
// if statements with initializers.
#include <fmt/format.h> // C++20: This will be #include <format> 
#include <iostream>
using namespace std;

int main() {
   if (int value{7}; value == 7) {
      cout << fmt::format("value is {}\n", value);
   }
   else {
      cout << fmt::format("value is not 7; it is {}\n", value);
   }

   if (int value{13}; value == 9) {
      cout << fmt::format("value is {}\n", value);
   }
   else {
      cout << fmt::format("value is not 9; it is {}\n", value);
   }
}

