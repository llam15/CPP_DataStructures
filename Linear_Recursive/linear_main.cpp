#include <iostream>
#include <cassert>
using namespace std;

// Return true if any of the array elements is negative, false
// otherwise.
bool anyNegative(const double a[], int n)
{
    if (n <= 0) {
        return false;
    }
    
    if (a[0] < 0) {
        return true;
    }
    
    return anyNegative(a+1, n-1);
}

// Return the number of negative elements in the array.
int countNegatives(const double a[], int n)
{
    int num = 0;
    
    if (n <= 0) {
        return 0;
    }
    
    if (a[0] < 0) {
        num++;
    }
    
    return num + countNegatives(a+1, n-1);
}

// Return the subscript of the first negative element in the array.
// If no element is negative, return -1.
int firstNegative(const double a[], int n)
{
    if (n <= 0) {
        return -1;
    }
    
    if (a[0] < 0) {
        return 0;
    }
    
    
    return 1+firstNegative(a+1, n-1);
    
}

// Return the subscript of the smallest element in the array.  If
// more than one element has the same smallest value, return the
// smallest subscript of such an element.  If the array has no
// elements to examine, return -1.
int indexOfMin(const double a[], int n)
{
    
    if (n <= 0) {
        return -1;
    }
    
    if (n == 1) {
        return 0;
    }
    
    int rest = 1 + indexOfMin(a+1, n-1);
    
    if (a[0] <= a[rest]) {
        return 0;
    }
    
    return rest;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) {
        return true;
    }
    if (n1 < n2 || n1 <= 0) {
        return false;
    }
    
    bool rest;
    
    if (a1[0] == a2[0]) {
        rest = includes(a1+1, n1-1, a2+1, n2-1);
    }
    
    else rest = includes(a1+1, n1-1, a2, n2);
    
    
    return rest;
}


int main()
{
    const int n = 5;
    double arr[n] = {9, 0, 1, 4, -3};
    double arr2[n] = {-1, 5, -4, 3, -2};
    
    assert(anyNegative(arr, n));
    assert(!anyNegative(arr, 0));
    assert(!anyNegative(arr, -1));
    assert(countNegatives(arr, n) == 1);
    assert(countNegatives(arr2, n) == 3);
    assert(firstNegative(arr, n) == 4);
    assert(firstNegative(arr2, n) == 0);
    assert(indexOfMin(arr, n) == 4);
    assert(indexOfMin(arr2, n) == 2);
    
    double arr3[7] = {10, 50, 40, 20, 50, 40, 30};
    double arr4[3] = {10, 20, 30};
    double arr5[3] = {50, 30, 20};
    double arr6[3] = {10, 20, 20};
    
    
    assert(includes(arr3, 7, arr4, 3));
    assert(!includes(arr3, 7, arr5, 3));
    assert(!includes(arr3, 7, arr6, 3));
    assert(includes(arr3, 7, arr6, 0));
    assert(includes(arr3, 7, arr6, -1));
    assert(!includes(arr3, -1, arr6, 3));


    
    
    cout << "All tests passed" << endl;
}

