#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** standard(int**, int**, int, int*);
void recursive(int** left, int** right);
void mtx_print(int** mtx, int row);
int** mtx_allocate(int row);
void mtx_free(int** mtx, int row);
int** mtx_add(int**, int ** right, int row,int* cnt);
int** mtx_subtract(int**, int **, int row,int* );
int** mtx_multiply(int** left, int** right, int row, int* cnt);
int** strassen(int**, int**, int, int*);

int main () {
	srand(time(NULL));
	int i,j;
	int** left_44 = mtx_allocate(4);
	int** right_44 = mtx_allocate(4);
	int** left_88 = mtx_allocate(8);
	int** right_88 = mtx_allocate(8);
	
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			left_44[i][j] = rand()%1000;
			right_44[i][j] = rand()%1000;
		}
	}

	for (i=0; i<8; i++) {
		for (j=0; j<8; j++) {
			left_88[i][j] = rand()%1000;
			right_88[i][j] = rand()%1000;
		}
	}
	int* cnt = (int*)malloc(sizeof(int));
	*cnt = 0;
	
	printf("[ 4 X 4 ]\nA :\n");
	mtx_print(left_44, 4);
	printf("B :\n");
	mtx_print(right_44,4);
	printf("\nC (Standard) :\n");
	mtx_print(standard(left_44, right_44, 4, cnt),4);
	printf("# of computations(standard) : %d\n", *cnt);
	printf("\nC (Recursive) :\n");
	*cnt = 0;
	mtx_print(mtx_multiply(left_44, right_44, 4, cnt),4);
	printf("# of computations(recursive) : %d\n", *cnt);
	printf("\nC (Strassen's method) :\n");
	*cnt=0;
	mtx_print(strassen(left_44, right_44,4, cnt),4);
	printf("# of computations(strassen's method) : %d\n", *cnt);

	printf("\n\n[ 8 X 8 ]\nA :\n");
	mtx_print(left_88, 8);
	printf("B :\n");
	mtx_print(right_88,8);
	printf("\nC (Standard) :\n");
	*cnt = 0;
	mtx_print(standard(left_88, right_88, 8, cnt),8);
	printf("# of computations(standard) : %d\n", *cnt);
	printf("\nC (Recursive) :\n");
	*cnt = 0;
	mtx_print(mtx_multiply(left_88, right_88, 8, cnt),8);
	printf("# of computations(recursive) : %d\n", *cnt);
	printf("\nC (Strassen's method) :\n");
	*cnt = 0;
	mtx_print(strassen(left_88, right_88,8, cnt),8);
	printf("# of computations(strassen's method) : %d\n", *cnt);

	return 0;
}

int** standard(int** left, int** right, int row, int* cnt) {
	int** ret = (int**)malloc(row*sizeof(int* ));
	for (int i=0; i<row; i++) {
		ret[i] = (int*)malloc(row*sizeof(int));
	}
	int i, j, k;
	for (i = 0; i < row; i ++) {
		for (j = 0; j < row; j ++) {
			ret[i][j] = 0;
			for (k = 0; k < row; k ++) {
				(*cnt)+=2;
				ret[i][j] += left[i][k] * right[k][j];
			}
			(*cnt)--;
		}
	}
	return ret;
}

void mtx_print(int** mtx, int row) {
	int i,j;
	for (i=0; i<row; i++){
		for (j=0; j<row; j++){
			printf("%d ",mtx[i][j]);
		}
		printf("\n");
	}
}

int** mtx_allocate(int row) {
	int** mtx = (int**)malloc(row*sizeof(int* ));
	for (int i=0; i<row; i++) {
		mtx[i] = (int*)calloc(row,sizeof(int));
	}
	return mtx;
}

void mtx_free(int** mtx, int row) {
	for (int i=0; i<row; i++)
		free(mtx[i]);
	free(mtx);
}

int** mtx_add(int** left, int ** right, int row, int* cnt) {
	int **ret = mtx_allocate(row);
	int i,j;
	for (i=0; i<row; i++) {
		for (j=0; j<row; j++) {
			(*cnt)++;
			ret[i][j] = left[i][j] + right[i][j];
		}
	}
	return ret;
}

int** mtx_subtract(int** left, int ** right, int row, int* cnt) {
	int **ret = mtx_allocate(row);
	int i,j;
	for (i=0; i<row; i++) {
		for (j=0; j<row; j++) {
			(*cnt)++;
			ret[i][j] = left[i][j] - right[i][j];
		}
	}
	return ret;
}

int** mtx_multiply(int** left, int** right, int row, int* cnt) {
	int **ret = mtx_allocate(row);
	int i,j;
	if (row == 1) {
		(*cnt)++;
		ret[0][0] = left[0][0]*right[0][0];
	} else {
		int subrow = row/2;
		int **left_a = mtx_allocate(subrow);
		int **left_b = mtx_allocate(subrow);
		int **left_c = mtx_allocate(subrow);
		int **left_d = mtx_allocate(subrow);
		int **right_a = mtx_allocate(subrow);
		int **right_b = mtx_allocate(subrow);
		int **right_c = mtx_allocate(subrow);
		int **right_d = mtx_allocate(subrow);

		for (i=0; i<subrow; i++) {
			for (j=0; j<subrow; j++) {
				left_a[i][j] = left[i][j];
				left_b[i][j] = left[i][j+subrow];
				left_c[i][j] = left[i+subrow][j];
				left_d[i][j] = left[i+subrow][j+subrow];
				right_a[i][j] = right[i][j];
				right_b[i][j] = right[i][j+subrow];
				right_c[i][j] = right[i+subrow][j];
				right_d[i][j] = right[i+subrow][j+subrow];
			}
		}

		int** ret_a = mtx_add(mtx_multiply(left_a, right_a, subrow, cnt),mtx_multiply(left_b, right_c, subrow, cnt), subrow, cnt);
		int** ret_b = mtx_add(mtx_multiply(left_a, right_b, subrow, cnt),mtx_multiply(left_b, right_d, subrow, cnt), subrow, cnt);
		int** ret_c = mtx_add(mtx_multiply(left_c, right_a, subrow, cnt),mtx_multiply(left_d, right_c, subrow, cnt), subrow, cnt);
		int** ret_d = mtx_add(mtx_multiply(left_c, right_b, subrow, cnt),mtx_multiply(left_d, right_d, subrow, cnt), subrow, cnt);
		
		for (i=0; i<subrow; i++) {
			for (j=0; j<subrow; j++) {
				ret[i][j] = ret_a[i][j];
				ret[i][j+subrow] = ret_b[i][j];
				ret[i+subrow][j] = ret_c[i][j];
				ret[i+subrow][j+subrow] = ret_d[i][j];
			}
		}
		mtx_free(ret_a,subrow);mtx_free(ret_b,subrow);mtx_free(ret_c,subrow);mtx_free(ret_d,subrow);mtx_free(left_a,subrow);mtx_free(left_b,subrow);mtx_free(left_c,subrow);mtx_free(left_d,subrow);mtx_free(right_a,subrow);mtx_free(right_b,subrow);mtx_free(right_c,subrow);mtx_free(right_d,subrow);
	}
	return ret;
}

int** strassen(int** left, int** right, int row, int* cnt) {
	int **ret = mtx_allocate(row);
	int i,j;
	if (row <=2) standard(left, right, row, cnt);
	else {
		int subrow = row/2;
		int **left_a = mtx_allocate(subrow);
		int **left_b = mtx_allocate(subrow);
		int **left_c = mtx_allocate(subrow);
		int **left_d = mtx_allocate(subrow);
		int **right_a = mtx_allocate(subrow);
		int **right_b = mtx_allocate(subrow);
		int **right_c = mtx_allocate(subrow);
		int **right_d = mtx_allocate(subrow);

		int ** m1 = mtx_allocate(subrow);
		int ** m2 = mtx_allocate(subrow);
		int ** m3 = mtx_allocate(subrow);
		int ** m4 = mtx_allocate(subrow);
		int ** m5 = mtx_allocate(subrow);
		int ** m6 = mtx_allocate(subrow);
		int ** m7 = mtx_allocate(subrow);	
		
		for (i=0; i<subrow; i++) {
			for (j=0; j<subrow; j++) {
				left_a[i][j] = left[i][j];
				left_b[i][j] = left[i][j+subrow];
				left_c[i][j] = left[i+subrow][j];
				left_d[i][j] = left[i+subrow][j+subrow];
				right_a[i][j] = right[i][j];
				right_b[i][j] = right[i][j+subrow];
				right_c[i][j] = right[i+subrow][j];
				right_d[i][j] = right[i+subrow][j+subrow];
			}
		}
		m1 = strassen(mtx_add(left_a, left_d, subrow, cnt),mtx_add(right_a, right_d, subrow, cnt), subrow, cnt);
		m2 = strassen(mtx_add(left_c, left_d, subrow, cnt),right_a, subrow, cnt);
		m3 = strassen(left_a, mtx_subtract(right_b,right_d,subrow,cnt), subrow, cnt);
		m4 = strassen(left_d, mtx_subtract(right_c, right_a,subrow, cnt), subrow, cnt);
		m5 = strassen(mtx_add(left_a, left_b, subrow, cnt), right_d, subrow, cnt);
		m6 = strassen(mtx_subtract(left_c, left_a, subrow, cnt), mtx_add(right_a, right_b, subrow, cnt),subrow, cnt);
		m7 = strassen(mtx_subtract(left_b, left_d, subrow, cnt), mtx_add(right_c, right_d, subrow, cnt),subrow, cnt);

		int** ret_a = mtx_add(mtx_subtract(mtx_add(m1,m4,subrow,cnt),m5,subrow, cnt), m7, subrow, cnt);
		int** ret_b = mtx_add(m3, m5, subrow, cnt);
		int** ret_c = mtx_add(m2, m4, subrow, cnt);
		int** ret_d =	mtx_add(mtx_subtract(mtx_add(m1,m3,subrow,cnt),m2,subrow, cnt), m6, subrow, cnt);

		for (i=0; i<subrow; i++) {
			for (j=0; j<subrow; j++) {
				ret[i][j] = ret_a[i][j];
				ret[i][j+subrow] = ret_b[i][j];
				ret[i+subrow][j] = ret_c[i][j];
				ret[i+subrow][j+subrow] = ret_d[i][j];
			}
		}
		mtx_free(ret_a,subrow);	mtx_free(ret_b,subrow);mtx_free(ret_c,subrow);mtx_free(ret_d,subrow);mtx_free(m1,subrow);mtx_free(m2,subrow);mtx_free(m3,subrow);mtx_free(m4,subrow);mtx_free(m5,subrow);mtx_free(m6,subrow);mtx_free(m7,subrow);mtx_free(left_a,subrow);mtx_free(left_b,subrow);mtx_free(left_c,subrow);mtx_free(left_d,subrow);mtx_free(right_a,subrow);mtx_free(right_b,subrow);mtx_free(right_c,subrow);mtx_free(right_d,subrow);

		return ret;

	}
}



























