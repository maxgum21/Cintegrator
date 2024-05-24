

struct sheap {
	char **arr;
	size_t size, max_size;
};

struct sheap *create_sheap();

void free_sheap(struct sheap *heap);

void push_sheap(char *str, struct sheap *heap);

char *pop_sheap(struct sheap *heap);


struct dheap {
	double *arr;
	size_t size, max_size;
};

struct dheap *create_dheap();

void free_dheap(struct dheap *heap);


void push_dheap(double val, struct dheap *heap);

double pop_dheap(struct dheap *heap);

int search_dheap(double key, struct dheap *heap);
