#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_BUCKETS 7

struct Elem {
	struct Elem* next;
	int key;
	int val;
};

struct List {
	struct Elem* start;
	struct Elem* end;
	int len;
};

struct List hash_table[NUM_BUCKETS];

void insert(int key, int val) {
	struct Elem* elem = malloc(sizeof(struct Elem));
	elem->key = key;
	elem->val = val;
	elem->next = NULL;

	int bucket_id = key % NUM_BUCKETS;
	struct List* bucket = &hash_table[bucket_id];
	bucket->len ++;

	if (bucket->start == NULL) {
		bucket->start = bucket->end = elem;
	} else {
		bucket->end->next = elem;
		bucket->end = elem;
	}
}

void delete(int key) {
	struct List* bucket = &hash_table[key % NUM_BUCKETS];
	struct Elem* elem = bucket->start;
	struct Elem* prev = NULL;
	if (bucket->len == 0)
		return;

	while (elem != NULL) {
		if (elem->key == key) {
			break;
		}
		prev = elem;
		elem = elem->next;
	}
	if (elem == NULL)
		return;

	if (prev == NULL) {
		bucket->start = elem->next;
	} else {
		prev->next = elem->next;
	}
	free(elem);
	bucket->len --;
}

int main() {
	int i = 0, j;
	char visit[100] = {0,};
	srand(time(NULL));
	while (i < 50) {
		int key = rand() % 100;
		if (!visit[key]) {
			insert(key, key);
			visit[key] = 1;
			i ++;
		}
	}
	printf("initial hash table\n");
	for (i = 0; i < NUM_BUCKETS; i ++) {
		struct List* bucket = &hash_table[i];
		struct Elem* elem = bucket->start;
		printf("bucket[%d] (%d) : ", i, bucket->len);
		for (j = 0; j < bucket->len; j ++) {
			printf("(%d, %d) ", elem->key, elem->val);
			elem = elem->next;
		}
		printf("\n");
	}

	for (i = 1; i <= 10; i ++) {
		delete(i);
	}

	printf("\nhash table after deleting 1-10\n");
	for (i = 0; i < NUM_BUCKETS; i ++) {
		struct List* bucket = &hash_table[i];
		struct Elem* elem = bucket->start;
		printf("bucket[%d] (%d) : ", i, bucket->len);
		for (j = 0; j < bucket->len; j ++) {
			printf("(%d, %d) ", elem->key, elem->val);
			elem = elem->next;
		}
		printf("\n");
	}

	return 0;
}
