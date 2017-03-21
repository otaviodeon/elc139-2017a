#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int max(int a, int b) { return (a > b)? a : b; }
int max2(int a, int b, int c) { return max(max(a, b), c); }

// Find the maximum possible sum in arr[] auch that arr[m] is part of it
int maxCrossingSum(int arr[], int low, int mid, int high) {
    int sum = 0;
    int left_sum = INT_MIN;
    for (int i = mid; i >= low; i--) {
        sum = sum + arr[i];
        if (sum > left_sum)
          left_sum = sum;
    }
    sum = 0;
    int right_sum = INT_MIN;
    for (int i = mid+1; i <= high; i++) {
        sum = sum + arr[i];
        if (sum > right_sum)
          right_sum = sum;
    }
    return left_sum + right_sum;
}

int maxSubArraySum(int arr[], int low, int high) {
   if (low == high)
     return arr[low];
   int mid = (low + high)/2;
   return max2(maxSubArraySum(arr, low, mid), maxSubArraySum(arr, mid+1, high), maxCrossingSum(arr, low, mid, high));
}

int *fill_array(int *A, int n) {
	A = malloc(n*sizeof(int));
	for (int i=0; i<n; i++) {
		A[i] = rand() % 41 + (-20);
	}
	return A;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	int *A;
    int n = atoi(argv[1]);
	A = fill_array(A, n);
    //for (int i=0; i<n; i++)
    //    printf("%d ", A[i]);
    int max = maxSubArraySum(A, 0, n-1);
    printf("Soma: %d\n", max);
	free(A);
}
