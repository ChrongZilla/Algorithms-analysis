#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <chrono>

using namespace std;

void InsertionSort(vector<int>& A);
void MergeSort(vector<int>& A, int left, int right);
void Merge(vector<int>& A, int left, int mid, int right);

void GenerateRandomIntegers(vector<int>& A);
bool IsSorted(const vector<int>& A, bool ascendingOrder);
void CheckTheResult(const vector<int>& A, bool ascendingOrder);
void PrintArray(const vector<int>& A);
long long GetCurrentUsec();

int main() {
    int n = 100; // change this number
    vector<int> A(n);
    GenerateRandomIntegers(A);

    vector<int> B = A;

    // print input
    // cout << "Input : ";
    // PrintArray(A);
    // CheckTheResult(A, true);

    long long t1, t2;

    // Insertion Sort
    t1 = GetCurrentUsec();
    InsertionSort(A);
    t2 = GetCurrentUsec();
    // cout << "Output : ";
    // PrintArray(A);
    CheckTheResult(A, true);
    cout << "It took " << (t2 - t1) << " usec by Insertion Sort" << endl;

    // Merge Sort
    t1 = GetCurrentUsec();
    MergeSort(B, 0, n - 1);
    t2 = GetCurrentUsec();
    // cout << "Output : ";
    // PrintArray(B);
    CheckTheResult(B, true);
    cout << "It took " << (t2 - t1) << " usec by Merge Sort" << endl;

    return 0;
}

void GenerateRandomIntegers(vector<int>& A) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int& x : A) {
        x = rand() % 100;
    }
}

void InsertionSort(vector<int>& A) {
    // your code here
    int n = A.size();
    for(int i = 1; i < A.size(); i++) { // A[0] is already sorted.
        int key = A[i];
        int j = i - 1;

        while (j > 0 && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }

        A[j + 1] = key;
    }
}

void MergeSort(vector<int>& A, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        MergeSort(A, left, mid);
        MergeSort(A, mid + 1, right);
        Merge(A, left, mid, right);
    }
}

void Merge(vector<int>& A, int left, int mid, int right) {
    // your code here
    int i = left;
    int j = mid + 1;
    int m = 0;
    int n = right - left + 1; 
    vector<int> M(n);

    while(i <= mid && j <= right) {
        if(A[i] <= A[j]){
            M[m] = A[i];
            i++;
        } else {
            M[m] = A[j];
            j++;
        }
        m++;
    }
    while(i <= mid){
        M[m] = A[i];
        i++;
        m++;
    }
    while(j <= right){
        M[m] = A[j];
        j++;
        m++;
    }
    for(int k = 0; k < n; k++) {
        A[left + k] = M[k];
    }

}

void PrintArray(const vector<int>& A) {
    cout << "{";
    for (int x : A) {
        cout << x << ", ";
    }
    cout << "}" << endl;
}

long long GetCurrentUsec() {
    auto now = chrono::high_resolution_clock::now();
    auto us = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch());
    return us.count();
}

void CheckTheResult(const vector<int>& A, bool ascendingOrder) {
    cout << "The result is ";
    if (!IsSorted(A, ascendingOrder)) {
        cout << "not ";
    }
    cout << "sorted in ";
    if (ascendingOrder) {
        cout << "ascending";
    } else {
        cout << "descending";
    }
    cout << " order." << endl;
}

bool IsSorted(const vector<int>& A, bool ascendingOrder) {
    int n = static_cast<int>(A.size());
    for (int i = 0; i < n - 1; i++) {
        if (ascendingOrder) {
            if (A[i] > A[i + 1]) return false;
        } else {
            if (A[i] < A[i + 1]) return false;
        }
    }
    return true;
}

