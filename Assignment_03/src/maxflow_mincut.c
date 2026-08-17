#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "csr_conversion.h"
#include "maxflow_mincut.h"


static int *head;
static int *edgeNext;
static int *edgeTo;
static int *edgeCap;
static int edgeCount;

static int *level;
static int *curArc;     /* "next edge to try" pointer per vertex, for DFS */
static int *queueArr;

static void addEdge(int u, int v, int cap) {
    /* forward edge: u -> v with given capacity */
    edgeTo[edgeCount] = v;
    edgeCap[edgeCount] = cap;
    edgeNext[edgeCount] = head[u];
    head[u] = edgeCount;
    edgeCount++;

    /* reverse edge: v -> u with 0 capacity  */
    edgeTo[edgeCount] = u;
    edgeCap[edgeCount] = 0;
    edgeNext[edgeCount] = head[v];
    head[v] = edgeCount;
    edgeCount++;
}

/* BFS: label every vertex with its distance from source */
static int bfs(int vertices, int source, int sink) {
    for (int i = 0; i < vertices; i++)
        level[i] = -1;
    level[source] = 0;

    int front = 0, back = 0;
    queueArr[back++] = source;

    while (front < back) {
        int u = queueArr[front++];
        for (int eid = head[u]; eid != -1; eid = edgeNext[eid]) {
            int v = edgeTo[eid];
            if (edgeCap[eid] > 0 && level[v] == -1) {
                level[v] = level[u] + 1;
                queueArr[back++] = v;
            }
        }
    }
    return level[sink] != -1;
}

/* DFS: push flow along the level graph, from u towards sink. */
static int dfs(int u, int sink, int pushed) {
    if (u == sink || pushed == 0)
        return pushed;

    for (; curArc[u] != -1; curArc[u] = edgeNext[curArc[u]]) {
        int eid = curArc[u];
        int v = edgeTo[eid];

        /* only move to vertices exactly one level ahead, with spare capacity */
        if (edgeCap[eid] > 0 && level[v] == level[u] + 1) {
            int canPush = (pushed < edgeCap[eid]) ? pushed : edgeCap[eid];
            int actuallyPushed = dfs(v, sink, canPush);

            if (actuallyPushed > 0) {
                edgeCap[eid] -= actuallyPushed;         /* use up forward capacity */
                edgeCap[eid ^ 1] += actuallyPushed;      /* give it back on the reverse edge */
                return actuallyPushed;
            }
        }
    }
    return 0; /* no path found through u in this phase */
}

MaxflowResult dinicMaxflow(CSRGraph *graph, int source, int sink) {
    int vertices = graph->vertices;
    int maxEdgeSlots = 2 * graph->edges; /* forward + reverse for every directed edge */

    head = malloc(vertices * sizeof(int));
    edgeNext = malloc(maxEdgeSlots * sizeof(int));
    edgeTo = malloc(maxEdgeSlots * sizeof(int));
    edgeCap = malloc(maxEdgeSlots * sizeof(int));
    level = malloc(vertices * sizeof(int));
    curArc = malloc(vertices * sizeof(int));
    queueArr = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++)
        head[i] = -1;
    edgeCount = 0;

    /* Build the residual graph from the CSR input. */
    for (int u = 0; u < vertices; u++) {
        for (int idx = graph->rowPtr[u]; idx < graph->rowPtr[u + 1]; idx++) {
            int v = graph->colIndex[idx];
            int cap = graph->edgeWeights[idx];
            addEdge(u, v, cap);
        }
    }

    long long totalFlow = 0;

    /* Main Dinic loop: keep finding level graphs and blocking flows
     * until sink is no longer reachable. */
    while (bfs(vertices, source, sink)) {
        for (int i = 0; i < vertices; i++)
            curArc[i] = head[i];

        int pushed;
        while ((pushed = dfs(source, sink, INT_MAX)) > 0)
            totalFlow += pushed;
    }

    /*Min cut extraction */
   
    int *visited = calloc(vertices, sizeof(int));
    int front = 0, back = 0;
    queueArr[back++] = source;
    visited[source] = 1;

    while (front < back) {
        int u = queueArr[front++];
        for (int eid = head[u]; eid != -1; eid = edgeNext[eid]) {
            int v = edgeTo[eid];
            if (edgeCap[eid] > 0 && !visited[v]) {
                visited[v] = 1;
                queueArr[back++] = v;
            }
        }
    }

    MaxflowResult res;
    res.maxFlow = totalFlow;

    res.sourceSide = malloc(vertices * sizeof(int));
    res.sinkSide = malloc(vertices * sizeof(int));
    res.sourceSideCount = 0;
    res.sinkSideCount = 0;

    for (int i = 0; i < vertices; i++) {
        if (visited[i])
            res.sourceSide[res.sourceSideCount++] = i;
        else
            res.sinkSide[res.sinkSideCount++] = i;
    }

    /* Cutting edges: original edges from source side to sink side. */
    res.cutFrom = malloc(graph->edges * sizeof(int));
    res.cutTo = malloc(graph->edges * sizeof(int));
    res.cutCap = malloc(graph->edges * sizeof(int));
    res.cutEdgeCount = 0;

    for (int u = 0; u < vertices; u++) {
        if (!visited[u]) continue;
        for (int eid = head[u]; eid != -1; eid = edgeNext[eid]) {
            if (eid % 2 != 0) continue; /* skip reverse edges */
            int v = edgeTo[eid];
            if (!visited[v]) {
                /* recover original capacity: cap left + flow already pushed */
                int originalCap = edgeCap[eid] + edgeCap[eid ^ 1];
                res.cutFrom[res.cutEdgeCount] = u;
                res.cutTo[res.cutEdgeCount] = v;
                res.cutCap[res.cutEdgeCount] = originalCap;
                res.cutEdgeCount++;
            }
        }
    }

    free(visited);
    free(head);
    free(edgeNext);
    free(edgeTo);
    free(edgeCap);
    free(level);
    free(curArc);
    free(queueArr);

    return res;
}

void freeMaxflowResult(MaxflowResult *res) {
    free(res->sourceSide);
    free(res->sinkSide);
    free(res->cutFrom);
    free(res->cutTo);
    free(res->cutCap);
}