#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 99

int merge_sort(int* arr, int left, int right);
int merge_merge(int* arr, int left, int mid, int right);	
void sorted(int*);
void merge_print(int* arr);	
void reverse_sorted(int*);

int main() {
	srand(time(NULL));
	int* A = malloc(sizeof(int)*(MAX+1));
	int visit[1001]={0,};
	for (int i=0; i<MAX; i++ ){
		do {
			A[i] = rand()%1000;
		} while (visit[A[i]] == 1);
		visit[A[i]] = 1;
	}

	printf("random case:\n");
	merge_print(A);
	sorted(A);
	printf("sorted case:\n");
	merge_print(A);
	reverse_sorted(A);
	printf("reversely sorted case:\n");
	merge_print(A);

	return 0;
}
void sorted(int* arr) { 
	for (int i=0; i<MAX; i++)
		arr[i]=i+1;
}
void reverse_sorted(int* arr) {
	for (int i=0; i<MAX; i++)
		arr[i]=100-i;
}


int merge_merge(int* arr, int left, int mid, int right){
	int cnt=0, first=left, second=mid+1, i=left;
	int sorted_arr[MAX+1] = {0,};
	while (first <= mid && second <= right) {
		cnt++;
		if (arr[first]<=arr[second])
			sorted_arr[i++] = arr[first++];
		else
			sorted_arr[i++] = arr[second++];
	}
	if (first > mid) {
		for (int j=second; j<=right; j++)
			sorted_arr[i++] = arr[j];
	}
	else {
		for (int j=first; j<=mid; j++)
			sorted_arr[i++] = arr[j];
	}

	for (int j=left; j<=right; j++)
		arr[j] = sorted_arr[j];
	
	return cnt;
}

int merge_sort(int* arr, int left, int right) {
	int cnt=0;
	if (left<right) {
		int mid = (left + right)/2;
		cnt += merge_sort(arr, left, mid);
		cnt += merge_sort(arr,mid+1, right);
		cnt += merge_merge(arr,left,mid,right);
	}
	return cnt;
}


void merge_print(int* arr) {
	int cnt;
	for (int i=0; i<MAX; i++) {
		printf("%4d",arr[i]);
	}	printf("\n");
	cnt = merge_sort(arr, 0, MAX-1);
	for (int i=0; i<MAX; i++) {
		printf("%4d",arr[i]);
	}	printf("\n# of comparisons : %d\n", cnt);
}
