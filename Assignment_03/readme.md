# Assignment 03

**Title:** Gradient Descent and Maxflow-Mincut

**Type:** Buddy

**Objective:** Implement Gradient Descent (generic one-variable polynomial optimizer) and Maxflow-Mincut (Dinic's algorithm on a CSR representation of a directed capacity graph), and measure each algorithm's execution time separately from input parsing / CSR conversion (setup) time.

## Explanation of the Algorithm / Approach

**Gradient Descent** — Iteratively minimizes a one-variable polynomial `f(x) = c0 + c1*x + c2*x^2 + ... + cd*x^d` by repeatedly moving `x` in the direction that decreases `f(x)`, using the update rule `x_new = x - learning_rate * f'(x)`. The derivative `f'(x) = c1 + 2*c2*x + ... + d*cd*x^(d-1)` is computed directly from the coefficient array, so one implementation works for every required degree (2, 4, 6, 8, 10) without hard-coding a separate function per test case. Stops when `|f'(x)| <= tolerance` or when `MAX_ITERATIONS` is reached. Gradient Descent does not use CSR — it reads its polynomial coefficients and scalar parameters directly from its input file.

**Maxflow-Mincut (Dinic's Algorithm)** — Finds the maximum flow from a source vertex `s` to a sink vertex `t` in a directed, capacity-weighted graph. Builds a residual graph from the CSR input (every edge gets a reverse edge with 0 capacity, so flow can be "undone" later). Repeatedly runs BFS to build a level graph (labels each vertex with its distance from `s`, using only edges with spare capacity), then DFS to push a blocking flow through that level graph, until `t` is no longer reachable. By the max-flow min-cut theorem, the total flow found equals the capacity of the minimum s-t cut. The minimum cut itself is recovered after the algorithm finishes, by finding all vertices still reachable from `s` in the final residual graph — that set is the source side, everything else is the sink side, and edges crossing from source side to sink side are the cut edges.

## Input Format, Assumptions, and Constraints

**Gradient Descent** — polynomial and optimizer parameters:
```
DEGREE d
COEFFICIENTS c0 c1 c2 ... cd
INITIAL_X x0
LEARNING_RATE alpha
TOLERANCE epsilon
MAX_ITERATIONS n
```

**Maxflow-Mincut** — directed capacity adjacency list:
```
V E
u0 degree neighbor1 capacity1 neighbor2 capacity2 ...
u1 degree neighbor1 capacity1 neighbor2 capacity2 ...
...
u(V-1) degree neighbor1 capacity1 neighbor2 capacity2 ...
SOURCE s
SINK t
```

- `V` is the number of vertices, `E` the number of directed capacity edges. Only outgoing edges are listed per vertex.
- A vertex with no outgoing edges is written as `u 0`.
- Capacities are positive integers for all required tests.
- `s` and `t` must be different, valid vertices, and at least one source-to-sink path must exist.
- Reverse residual edges are **not** written in the input file — the max-flow implementation creates and updates them internally.
- The `COEFFICIENTS` line for Gradient Descent must contain exactly `d + 1` values, constant term first, coefficient of `x^d` last.

**Assumptions:**
- Vertices are listed in order (0 to V-1) in the Maxflow-Mincut input file, matching the CSR-conversion convention from Assignment 01.
- Gradient Descent coefficients, `x0`, learning rate, and tolerance may be floating-point; everything else (degree, iteration counts, vertex/edge counts, capacities) is an integer.
- The input file is well-formed unless it violates one of the checks below.

**Constraints:**
- Maxflow-Mincut `colIndex` / `edgeWeights` are allocated at size `E` (directed graph — one entry per listed edge, not doubled).
- The residual graph used internally by Dinic's algorithm allocates `2 x E` edge slots (forward + reverse per input edge).


## Source Files, Driver Files, Helper Functions, and Test Files

**`../utils/csr_conversion.c` / `csr_conversion.h`** (from Assignment 01, reused — not copied):
- `readAdjacencyListAsCSR(filename, isWeighted, isDirected)` — reads the adjacency list and builds `rowPtr`, `colIndex`, and `edgeWeights` in a single pass.
- `readSourceVertex(filename)` — reads and validates the trailing `SOURCE s` line.
- `printCSRGraph(graph)` / `freeCSRGraph(graph)`.

**`../utils/graph_helper.c` / `graph_helper.h`**:
- `sortNeighborLists(graph)` — sorts the neighbor lists of each vertex in ascending order using `qsort`.
- `hasEdge(graph, u, v)` — performs a binary search to check whether a directed edge `(u, v)` exists in the CSR structure.
- `validateUndirectedSymmetry(graph)` — checks that the input file describes a proper undirected graph (i.e., verifies every edge is listed symmetrically on both ends).

**`src/maxflow_mincut.c` / `maxflow_mincut.h`** — `dinicMaxflow(graph, source, sink)` builds the residual graph from CSR and runs Dinic's algorithm, returning max flow value, minimum cut capacity, both sides of the cut, and the cut edges; `readSinkVertex(filename)` — reads and validates the trailing `SINK t` line; `freeMaxflowResult(result)` frees them

**`src/gradient_descent.c` / `gradient_descent.h`** — `runGradientDescent(...)` evaluates `f(x)` / `f'(x)` from the coefficient array and applies the update rule until convergence or `MAX_ITERATIONS`; `freeGradientDescentResult(result)` frees the result.

**`driver/driver.c`** — contains `main()`. Parses the input file path and algorithm name (`gd` or `maxflow`), and dispatches to the matching runner.

**`driver/maxflow_mincut_runner.c`** — opens the input file, converts it to CSR (untimed), reads source and sink, times only the `dinicMaxflow` call, prints the result, and frees all allocated memory.

**`driver/gd_runner.c`** — opens the input file, parses the polynomial/optimizer parameters (untimed), times only the `runGradientDescent` call, prints the result, and frees all allocated memory.

**`../common_wrapper/wrapper.c`** (shared across all assignments):
- `openFile(filename)`, `getExecutionTime()`, `printExecutionTime(start, end)`.

**`tests/`** — input files at the required sizes:
- `maxflow_<V>.txt` — directed capacity adjacency list, for V in {10, 100, 1000, 10000, 50000, 100,000}.
- `gd_01.txt` ... `gd_05.txt` — polynomial degrees 2, 4, 6, 8, 10 respectively.

## Compilation and Execution Instructions

From inside `Assignment_03/`:

```bash
cd Assignment_03
```

**Compile:**
```bash
make
```

**Run Gradient Descent / Maxflow-Mincut:**
```bash
./run tests/gd_01.txt gd
./run tests/maxflow_10.txt maxflow
```

**Clean up the compiled binary:**
```bash
make clean
```

## Gradient Descent Results Table

| File | Degree | x0 | Learning Rate | Tolerance | Max Iter. | Expected x* | Expected f(x*) | Actual x | Actual f(x) | Iterations | Time | Status |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| gd_01.txt | 2 | 0 | 0.10 | 1e-6 | 5,000 | 3 | 0 |  3 | 0 | 70 | 0.001020 ms | Completed |
| gd_02.txt | 4 | 2 | 0.02 | 1e-6 | 10,000 | 0 | 0 | 0 | 0 | 180 | 0.003850 ms | Completed |
| gd_03.txt | 6 | 2 | 0.02 | 1e-6 | 20,000 | 0 | 0 | 0 | 0 | 349 | 0.011810 ms | Completed |
| gd_04.txt | 8 | 2 | 0.01 | 1e-8 | 50,000 | 0 | 0 | 0 | 0 | 948 | 0.037940 ms | Completed |
| gd_05.txt | 10 | 2 | 0.005 | 1e-10 | 100,000 | 0 | 0 | 0 |0 | 2364 | 0.137690 ms | Completed |

## Maxflow-Mincut Results Table

| File | V | E | Source | Sink | Actual Flow | Cut Capacity | Time | Status |
|---|---|---|---|---|---|---|---|---|
| maxflow_10.txt | 10 | 20 | 0 | 9 | 19 | 19 | 0.0100 ms | Completed |
| maxflow_100.txt | 100 | 300 | 0 | 99 | 19 | 19 | 0.0600 ms | Completed |
| maxflow_1000.txt | 1,000 | 3,000 | 0 | 999 | 21 | 21 | 0.6020 ms | Completed |
| maxflow_10000.txt | 10,000 | 30,000 | 0 | 9999 | 27 | 27 | 10.9540 ms | Completed |
| maxflow_50000.txt | 50,000 | 150,000 | 0 | 49999 | 25 | 25 | 51.7660 ms | Completed |
| maxflow_100000.txt (optional) | 100,000 | 300,000 | 0 | 99999 | 23 | 23 | 287.4210 ms | Completed |

## Time and Space Complexity

**Gradient Descent:** O(n · d) time, where `n` is the number of iterations run and `d` is the polynomial degree (each iteration evaluates `f'(x)` in O(d)). O(d) space for the coefficient array.

**Maxflow-Mincut (Dinic's Algorithm):** O(V² · E) time in the general case — each of the O(V) BFS phases finds a level graph in O(V + E), and the blocking-flow DFS phase across all phases does O(V · E) work total. O(V + E) space for the residual graph (adjacency lists sized `2E` for forward + reverse edges) plus O(V) for the level, current-arc, and BFS-queue arrays.