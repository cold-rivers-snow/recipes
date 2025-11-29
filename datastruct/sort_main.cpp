#include "shell_sort.h"
#include "insert_sort.h"
#include "bubble_sort.h"
#include "select_sort.h"
#include "quick_sort.h"

void test_select_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    selectSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_bubble_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    bubbleSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_insert_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    insertSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_shell_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    shellSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_quick_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    quickSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

int main() {
    test_select_sort();
    test_bubble_sort();
    test_insert_sort();
    test_shell_sort();
    test_quick_sort();
    return 0;
}