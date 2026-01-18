// fig04_17.cpp
// Compound-interest example with boost::multiprecision::cpp_dec_float_50. 
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <format> 
#include <iostream>
#include "decimalformatter.h" 

using namespace std;
using boost::multiprecision::cpp_dec_float_50;

int main() {
   cpp_dec_float_50 principal{1000}; // $1000 initial principal
   cpp_dec_float_50 rate{"0.05"}; // 5% interest rate
 
   cout << format("Initial principal: {:>7}\n", principal)
        << format("    Interest rate: {:>7}\n\n", rate);

   // display headers
   cout << format("{}{:>20}\n", "Year", "Amount on deposit");

   // calculate amount on deposit for each of 10 years
   for (int year{1}; year <= 10; ++year) {
      cpp_dec_float_50 amount{principal * pow(1 + rate, year)};
      cout << format("{:>4}{:>20}\n", year, amount);
   }
}
