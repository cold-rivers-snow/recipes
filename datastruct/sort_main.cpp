#include "shell_sort.h"
#include "insert_sort.h"
#include "bubble_sort.h"
#include "select_sort.h"
#include "quick_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "counting_sort.h"
#include "bucket_sort.h"
#include "recursive_bucket_sort.h"
#include "radix_sort.h"

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

void test_merge_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    mergeSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_heap_sort() {
    std::vector<int> arr = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    heapSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_count_sort() {
    std::vector<int> arr = {-5, -1, 0, 2, -1, 3};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    auto result = countingSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (int x : result) std::cout << x << " ";
    std::cout << "\n";  
}

void test_bucket_sort() {
    std::vector<double> arr = {-5, -1, 0, 2, -1, 3};
    std::cout << __func__ << " Original: ";
    for (double x : arr) std::cout << x << " ";
    std::cout << "\n";

    bucketSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (double x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_recursive_bucket_sort() {
    std::vector<double> arr = {-5, -1, 0, 2, -1, 3};
    std::cout << __func__ << " Original: ";
    for (double x : arr) std::cout << x << " ";
    std::cout << "\n";

    recursiveBucketSort(arr);

    std::cout  << __func__ << " Sorted:   ";
    for (double x : arr) std::cout << x << " ";
    std::cout << "\n";  
}

void test_radix_sort() {
    std::vector<int> arr = {-170, -45, 75, 90, -2, 802, 24, 2, 66};
    std::cout << __func__ << " Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    radixSort(arr);

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
    test_merge_sort();
    test_heap_sort();
    test_count_sort();
    test_bucket_sort();
    test_recursive_bucket_sort();
    test_radix_sort();
    return 0;
}