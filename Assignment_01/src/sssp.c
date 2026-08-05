#include <stdlib.h>
#include <limits.h>
#include "sssp.h"

typedef struct {
    int dist;
    int vertex;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

static MinHeap heap_create(int capacity) {
    MinHeap h;
    h.data = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    h.size = 0;
    h.capacity = capacity;
    return h;
}

static void heap_push(MinHeap *h, HeapNode node) {
    int i = h->size++;
    h->data[i] = node;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[parent].dist <= h->data[i].dist) break;
        HeapNode tmp = h->data[parent];
        h->data[parent] = h->data[i];
        h->data[i] = tmp;
        i = parent;
    }
}

static HeapNode heap_pop(MinHeap *h) {
    HeapNode top = h->data[0];
    h->data[0] = h->data[--h->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < h->size && h->data[left].dist < h->data[smallest].dist) smallest = left;
        if (right < h->size && h->data[right].dist < h->data[smallest].dist) smallest = right;
        if (smallest == i) break;
        HeapNode tmp = h->data[smallest];
        h->data[smallest] = h->data[i];
        h->data[i] = tmp;
        i = smallest;
    }
    return top;
}

void sssp_dijkstra(const CSRGraph *g, int source, int *dist_out) {
    for (int i = 0; i < g->vertices; i++) dist_out[i] = INT_MAX;
    dist_out[source] = 0;

    MinHeap h = heap_create(g->edges + g->vertices);  /* max possible size: all vertices + all edges */
    heap_push(&h, (HeapNode){0, source});

    while (h.size > 0) {
        HeapNode cur = heap_pop(&h);
        if (cur.dist > dist_out[cur.vertex]) continue; /* stale entry, skip */

        int start = g->rowPtr[cur.vertex];
        int end = g->rowPtr[cur.vertex + 1];
        for (int e = start; e < end; e++) {
            int neighbor = g->colIndex[e];
            int weight = g->edgeWeights[e];
            int newDist = cur.dist + weight;
            if (newDist < dist_out[neighbor]) {
                dist_out[neighbor] = newDist;
                heap_push(&h, (HeapNode){newDist, neighbor});
            }
        }
    }

    free(h.data);

    //Mark unreached vertices with SSSP_INF
    for (int i = 0; i < g->vertices; i++) {
        if (dist_out[i] == INT_MAX) dist_out[i] = SSSP_INF;
    }
}
