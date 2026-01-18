// fig05_03.cpp
// Rolling a six-sided die randomly 60,000,000 times.
#include <format>
#include <iostream>
#include <random> 
using namespace std;

int main() {
   // set up random-number generation
   default_random_engine engine{};
   uniform_int_distribution randomDie{1, 6};

   int frequency1{0}; // count of 1s rolled
   int frequency2{0}; // count of 2s rolled
   int frequency3{0}; // count of 3s rolled 
   int frequency4{0}; // count of 4s rolled
   int frequency5{0}; // count of 5s rolled
   int frequency6{0}; // count of 6s rolled

   // summarize results of 60,000,000 rolls of a die，单引号可以对数字分组，不会改变原有含义
   for (int roll{1}; roll <= 60'000'000; ++roll) {
      // determine roll value 1-6 and increment appropriate counter
      switch (const int face{randomDie(engine)}) { //C++17 正式支持在 switch 的判断表达式中直接定义变量（部分编译器如 GCC/Clang 在 C++11 后也兼容此写法），相比传统 “先定义变量再写 switch” 更简洁 // 传统写法（等价效果）const int face = randomDie(engine); switch (face) { ... }
         case 1:          
            ++frequency1; // increment the 1s counter
            break;
         case 2:          
            ++frequency2; // increment the 2s counter
            break;
         case 3:          
            ++frequency3; // increment the 3s counter
            break;
         case 4:         
            ++frequency4; // increment the 4s counter
            break;
         case 5:          
            ++frequency5; // increment the 5s counter
            break;
         case 6:         
            ++frequency6; // increment the 6s counter
            break;
         default: // invalid value
            cout << "Program should never get here!";
            break;
      } 
   } 

   cout << format("{:>4}{:>13}\n", "Face", "Frequency"); // headers
   cout << format("{:>4d}{:>13d}\n", 1, frequency1)
      << format("{:>4d}{:>13d}\n", 2, frequency2)
      << format("{:>4d}{:>13d}\n", 3, frequency3)
      << format("{:>4d}{:>13d}\n", 4, frequency4)
      << format("{:>4d}{:>13d}\n", 5, frequency5)
      << format("{:>4d}{:>13d}\n", 6, frequency6);
} 




/**************************************************************************
 * (C) Copyright 1992-2022 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 **************************************************************************/
