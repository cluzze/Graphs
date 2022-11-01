/*

Kruskal algorithm using edge list and dsu in O(sort(E) + (V + E)A), A - inverse Ackermann function

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct Edge
{
	int to, from;
	int weight;
} Edge;


/*************************************************** Vector ***************************************************/

typedef struct Vector
{
	int size;
	int capacity;
	Edge* edges;
} Vector;

Vector* initVector(Vector* vec, int size)
{
	if (!vec)
	{
		vec = (Vector*)calloc(1, sizeof(Vector));
	}

	vec->size = size;
	vec->capacity = size * 2;

	if (vec->capacity == 0)
	{
		vec->capacity += 2;
	}

	vec->edges = (Edge*)calloc(vec->capacity, sizeof(Edge));

	return vec;
}

void destroyVector(Vector* vec)
{
	free(vec->edges);
	free(vec);
}

void push_back(Vector* vec, Edge edge)
{
	if (vec->size == vec->capacity)
	{
		vec->capacity *= 2;
		vec->edges = (Edge*)realloc(vec->edges, vec->capacity * sizeof(Edge));
	}

	vec->edges[vec->size] = edge;
	vec->size++;
}

/***************************************************/

/*************************************************** DSU ***************************************************/

typedef struct DSU
{
	int* p;
	int* rank;
} DSU;

DSU* initDSU(DSU* dsu, int n)
{
	int i = 0;

	if (!dsu)
	{
		dsu = (DSU*)calloc(1, sizeof(DSU));
	}

	dsu->p = (int*)calloc(n, sizeof(int));
	dsu->rank = (int*)calloc(n, sizeof(int));

	for (i = 0; i < n; i++)
	{
		dsu->p[i] = i;
		dsu->rank[i] = 0;
	}

	return dsu;
}

void destroyDSU(DSU* dsu)
{
	free(dsu->p);
	free(dsu->rank);
	free(dsu);
}

int get(DSU* dsu, int i)
{
	return dsu->p[i] == i ? i : (dsu->p[i] = get(dsu, dsu->p[i]));
}

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void join(DSU* dsu, int a, int b)
{
	a = get(dsu, a);
	b = get(dsu, b);

	if (dsu->rank[b] <= dsu->rank[a])
		swap(&a, &b);

	if (dsu->rank[a] == dsu->rank[b])
		dsu->rank[b]++;

	dsu->p[a] = b;
}

/***************************************************/

int cmp(const void* a, const void* b)
{
	return ((const Edge*)a)->weight - ((const Edge*)b)->weight;
}

int main()
{
	int n = 0;
	DSU* dsu = NULL;
	Vector *edge_list = NULL;
	Vector *mst = NULL;
	int mst_weight = 0;
	Edge edge = {0, 0, 0};

	scanf("%d\n", &n);

	dsu = initDSU(dsu, n);

	edge_list = initVector(edge_list, 0);
	mst = initVector(mst, 0);

	while (scanf("%d %d %d", &edge.to, &edge.from, &edge.weight) == 3)
	{
		push_back(edge_list, edge);
	}

	qsort(edge_list->edges, edge_list->size, sizeof(Edge), cmp);

	for (int i = 0; i < edge_list->size; i++)
	{
		int to = edge_list->edges[i].to;
		int from = edge_list->edges[i].from;
		int weight = edge_list->edges[i].weight;

		if (get(dsu, to) != get(dsu, from))
		{
			mst_weight += weight;
			push_back(mst, edge_list->edges[i]);
			join(dsu, to, from);
		}
	}

	printf("%d\n", mst_weight);

	destroyDSU(dsu);
	destroyVector(mst);
	destroyVector(edge_list);

	return 0;
}
