#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int merge_sort(int*, int, int);
int merge_merge(int*, int, int, int);	
void merge_print(int*);	
int binary_search(int*, int, int, int*);
int linear_search(int*, int, int, int*);
int sorted_arr[101];

int main() {
	srand(time(NULL));
	int* A = malloc(sizeof(int)*101);
	int visit[1001]={0,};
	int cnt,key;
	for (int i=0; i<100; i++ ){
		do {
			A[i] = rand()%1000;
		} while (visit[A[i]] == 1);
		visit[A[i]] = 1;
	}
	merge_sort(A,0,99);
	/*printf("sorted case:\n");
	for (int i = 0; i < 100; i ++) {
		printf("%4d", A[i]);
	}	printf("\n");*/
	float avg_bcnt = 0, avg_lcnt = 0;
	for (int i = 0; i < 5; i ++) {
		key = rand() % 1000;
		cnt = 0;
		int idx = binary_search(A, 100, key, &cnt);
		avg_bcnt += cnt;
		printf("key(%d) is located at %d.\n", key, idx);
		printf("\t# of comparison with binary search: %d\n", cnt);
		cnt = 0;
		linear_search(A, 100, key, &cnt);
		avg_lcnt += cnt;
		printf("\t# of comparison with linear search: %d\n\n", cnt);
	}
	printf("* Average # of comparison with binary search: %.2f\n", avg_bcnt / 5);
	printf("* Average # of comparison with linear search: %.2f\n", avg_lcnt / 5);

	return 0;
}

int merge_merge(int* arr, int left, int mid, int right){
	int cnt=0, first=left, second=mid+1, i=left;
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


int binary_search(int* arr, int len, int key, int* cnt) {
	int left=0, mid, right=len-1;

	while (left<=right) {
		mid = (left+right)/2;
		(*cnt) ++;
		//printf("%d\n", mid);
		if (key > arr[mid]) {
			left = mid + 1;
			mid = (mid+right)/2;
		}
		else if (key < arr[mid]) {
			right = mid - 1;
			mid = (left+mid)/2;
		}
		else {
			return mid; 
		}
	}
	return -1;
}

int linear_search(int* arr, int len, int key, int* cnt) {
	int i;
	for (i=0; i<len; i++) {
		(*cnt)++;
		if (arr[i] == key) return i;
	}
	return -1;
}
