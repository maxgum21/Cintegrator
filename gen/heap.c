#include <stdlib.h>

struct sheap {
	char **arr;
	size_t size, max_size;
};

struct sheap *create_sheap() {
	return calloc(sizeof(struct sheap), 1);
}

void free_sheap(struct sheap *heap) {
	if (heap) free(heap);
}

void push_sheap(char *str, struct sheap *heap) {
	if (!heap->arr) {
		if (!heap->max_size) heap->max_size = 16;
		heap->arr = malloc(heap->max_size * sizeof(char *));
	}

	heap->arr[heap->size++] = str;

	if (heap->size == heap->max_size) {
		heap->max_size *= 2;
		heap->arr = realloc(heap->arr, sizeof(char *) * heap->max_size);
	}
}

char *pop_sheap(struct sheap *heap) {
	if (!heap->size) return NULL;

	char *temp = heap->arr[0];

	for (int i = 0; i < heap->size - 1; i++) {
		heap->arr[i] = heap->arr[i + 1];
	}

	heap->size--;
	return temp;
}

struct dheap {
	double *arr;
	size_t size, max_size;
};

struct dheap *create_dheap() {
	return calloc(sizeof(struct dheap), 1);
}

void free_dheap(struct dheap *heap) {
	if (heap) free(heap);
}

void push_dheap(double val, struct dheap *heap) {
	if (!heap->arr) {
		if (!heap->max_size) heap->max_size = 16;
		heap->arr = malloc(heap->max_size * sizeof(double ));
	}

	heap->arr[heap->size++] = val;

	if (heap->size == heap->max_size) {
		heap->max_size *= 2;
		heap->arr = realloc(heap->arr, sizeof(double ) * heap->max_size);
	}
}

double pop_dheap(struct dheap *heap) {
	double temp = heap->arr[0];

	for (int i = 0; i < (heap->size - 1); i++) {
		heap->arr[i] = heap->arr[i + 1];
	}

	heap->size--;
	return temp;
}

int search_dheap(double key, struct dheap *heap) {
	if (!heap->size) return -1;

	for (int i = 0; i < heap->size; i++) {
		if (heap->arr[i] == key) return i;
	}

	return -1;
}
