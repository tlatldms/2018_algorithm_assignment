#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int*);
void sorted(int*);
void reverse_sorted(int*);

int main() {
	srand(time(NULL));
	int* A = malloc(sizeof(int)*101);
	int visit[1001]={0,};
	for (int i=0; i<100; i++ ){
		do {
			A[i] = rand()%1000;
		} while (visit[A[i]] == 1);
		visit[A[i]] = 1;
	}

	printf("random case:\n");
	bubble_sort(A);
	sorted(A);
	printf("sorted case:\n");
	bubble_sort(A);
	reverse_sorted(A);
	printf("reversely sorted case:\n");
	bubble_sort(A);

	return 0;
}
void sorted(int* arr) { 
	for (int i=0; i<100; i++)
		arr[i]=i+1;
}
void reverse_sorted(int* arr) {
	for (int i=0; i<100; i++)
		arr[i]=100-i;
}

void bubble_sort(int* arr) {
	int i,j,tmp,cnt=0;
	for (int i=0; i<100; i++) {
		printf("%4d",arr[i]);
	}	printf("\n");
	for (i=0; i<99; i++) {
		for (j=0; j<99-i; j++){
			cnt++;
			if (arr[j] > arr[j+1]) {
				tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
	for (int i=0; i<100; i++) {
		printf("%4d",arr[i]);
	}	printf("\n# of comparisons : %d\n", cnt);
}
