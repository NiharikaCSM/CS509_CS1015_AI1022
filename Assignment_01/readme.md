# Assignment 01

**Title:** Graph Algorithms on CSR - BFS, DFS, SSSP (Dijkstra)

**Type:** Buddy

**Objective:** Implement BFS, DFS, and SSSP so that each operates on a Compressed Sparse Row (CSR) representation of the input graph, built from an adjacency-list input file, and measure each algorithm's execution time separately from the CSR conversion (setup) time.

## Explanation of the Algorithm / Approach

**CSR Conversion** — The adjacency-list file is read in a single pass and
converted into three arrays: `rowPtr`, `colIndex`, and (for weighted graphs) `edgeWeights`. `rowPtr[v]` to `rowPtr[v+1]` gives the range in `colIndex` holding vertex `v`'s neighbors. This conversion is always done before timing starts, since it is setup work, not part of the algorithm being measured.

**BFS** — Visits vertices level by level from the source, using an array-based queue. For each vertex dequeued, its neighbors are read directly from `colIndex[rowPtr[v] .. rowPtr[v+1])`. Reports the minimum edge-count distance from the source to every reachable vertex. Unreachable vertices are reported as `INF`.

**DFS** — Explores as deep as possible before backtracking. Implemented iteratively with an explicit stack (rather than recursively) to avoid stack-overflow risk on large graphs as recursion depth could reach 100,000 on the largest required test case. Neighbors are pushed in reverse order so the traversal visits the lowest-indexed neighbor first, matching what
a straightforward recursive DFS would produce.

**SSSP (Dijkstra)** — 

## Input Format, Assumptions, and Constraints

**BFS / DFS** — unweighted adjacency list:
```
V E
u0 degree neighbor1 neighbor2 ...
u1 degree neighbor1 neighbor2 ...
...
u(V-1) degree neighbor1 neighbor2 ...
SOURCE s
```

**SSSP** — positive-weighted adjacency list:
```
V E
u0 degree neighbor1 weight1 neighbor2 weight2 ...
u1 degree neighbor1 weight1 neighbor2 weight2 ...
...
u(V-1) degree neighbor1 weight1 neighbor2 weight2 ...
SOURCE s
```

- `V` is the number of vertices, `E` the number of unique edges (each undirected edge is counted once in `E`, even though it appears in both endpoints adjacency lists).
- A vertex with no neighbors is written as `u 0`.
- `SOURCE s` gives the source vertex for the algorithm.

**Assumptions:**
- Vertices are listed in order (0 to V-1) in the input file. This lets CSR conversion build `rowPtr` and `colIndex` in a single pass.
- Graphs are undirected; each edge appears once in each endpoint's adjacency list.
- For SSSP, all edge weights are positive integers.
- The input file is well-formed (degree counts match the actual number of neighbors listed).

**Constraints:**
- `colIndex` and `edgeWeights` are allocated at size `2 x E`, since each undirected edge contributes one entry to each endpoint's adjacency list.
- DFS uses an explicit stack sized to the total edge-entry count (a safe upper bound), rather than recursion, to handle graphs up to 100,000 vertices without stack-overflow risk.

## Input Validation
 
Since a malformed input file could otherwise cause a crash (segfault) or
silently produce incorrect output, `readAdjacencyListAsCSR` and
`readSourceVertex` validate the file as it's read and exit with a clear,
specific message rather than continuing on bad data. Checks include:
 
| Condition checked | Example error message |
|---|---|
| Header isn't two valid integers | `expected 'V E' on the first line, but couldn't read two integers.` |
| V <= 0 | `number of vertices (V) must be a positive integer.` |
| E < 0 | `number of edges (E) cannot be negative.` |
| Vertices not listed in order 0 to V-1 | `vertices must be listed in order 0 to V-1...` |
| Negative degree | `a vertex's degree cannot be negative.` |
| Total neighbors exceed what E implies | `total number of neighbors listed exceeds what E implies...` |
| File ends early / non-integer data found | `...the file ended early or contained non-integer data.` |
| Neighbor id out of range | `a neighbor id is out of range (must be between 0 and V-1).` |
| Zero or negative edge weight (SSSP) | `edge weights must be positive (SSSP requires this).` |
| Missing `SOURCE` line | `expected a 'SOURCE s' line at the end of the file.` |
| Wrong keyword instead of `SOURCE` | `expected the keyword 'SOURCE' before the source vertex id.` |

The `SOURCE` keyword check is **case-insensitive** (`strcasecmp`), so `SOURCE`, `source`, and `Source` are all accepted 

## Source Files, Driver Files, Helper Functions, and Test Files

**`../utils/csr_conversion.c` / `csr_conversion.h`** — CSR conversion:
- `readAdjacencyListAsCSR(filename, isWeighted)` — reads the adjacency list and builds `rowPtr`, `colIndex`, and (if weighted) `edgeWeights` in a single pass. ; exits with a descriptive error on any malformed input.
- `readSourceVertex(filename)` — reads and validates the trailing `SOURCE s` line.
- `printCSRGraph(graph)` — prints the CSR arrays (for verification).
- `freeCSRGraph(graph)` — frees the CSR arrays.

**`src/bfs.c` / `bfs.h`** — `runBFS(graph, source)` returns the traversal order and per-vertex distances; `freeBFSResult(result)` frees them.

**`src/dfs.c` / `dfs.h`** — `runDFS(graph, source)` returns the traversal order; `freeDFSResult(result)` frees it.

**`src/sssp.c` / `sssp.h`** — `runSSSP(graph, source)` returns per-vertex shortest distances; `freeSSSPResult(result)` frees them.

**`driver/driver.c`** — contains `main()`. Parses the input file path and algorithm name (`bfs`, `dfs`, or `sssp`), and dispatches to the matching runner.

**`driver/bfs_runner.c`, `dfs_runner.c`, `sssp_runner.c`** — each opens the input file, converts it to CSR (untimed), reads the source vertex, times only the algorithm call, prints the result, and frees all allocated memory.

**`../common_wrapper/wrapper.c`** (shared across all assignments):
- `openFile(filename)` — opens the input file, exits with an error if it can't be opened.
- `getExecutionTime()` — returns the current time.
- `printExecutionTime(start, end)` — prints the elapsed time in seconds.

**`tests/`** — adjacency-list input files at the required sizes:
- `test_<V>.txt` — unweighted, for V in {10, 100, 10000, 50000, 100000}.
- `sssp_<V>.txt` — positive-weighted, for V in {10, 100, 10000, 50000, 100000}.
- `bad_sample.txt` — the small, hand-verifiable 5-vertex examples to validate wrong input checks. 

## Compilation and Execution Instructions

From inside `Assignment_01/`:

**Compile:**
```bash
make
```

**Run BFS / DFS / SSSP:**
```bash
./run tests/bfs_dfs_sample.txt bfs
./run tests/bfs_dfs_sample.txt dfs
./run tests/sssp_sample.txt sssp
```

**Clean up the compiled binary:**
```bash
make clean
```

## Graph Results Table

| Algorithm | Test File | Vertices | Edges | Input Type | Source | Expected Output | Actual Output | Time | Status |
|---|---|---|---|---|---|---|---|---|---|
| BFS | test_01.txt | 10 | 30 | Unweighted adjacency list | 0 | Traversal / distances | Traversal / distances | 0.003000 ms | Pass |
| DFS | test_01.txt | 10 | 30 | Unweighted adjacency list | 0 | Traversal | Traversal | 0.004000 ms | Pass |
| BFS | test_02.txt | 100 | 300 | Unweighted adjacency list | 0 | Traversal / distances | Traversal / distances | 0.006000 ms | Pass |
| DFS |test_02.txt | 100 | 300 | Unweighted adjacency list | 0 | Traversal | Traversal | 0.014000 ms | Pass |
| BFS | test_03.txt | 100 | 150 | Unweighted adjacency list | 99 | Traversal / distances | Traversal / distances | 0.008000 ms | Pass |
| DFS | test_03.txt | 100 | 150 | Unweighted adjacency list | 99 | Traversal | Traversal | 0.007000 ms | Pass |
| BFS | test_04.txt | 10000 | 30000 | Unweighted adjacency list | 0 | Traversal / distances | Traversal / distances | 0.704000 ms | Pass |
| DFS | test_04.txt | 10000 | 30000 | Unweighted adjacency list | 0 | Traversal | Traversal | 1.058000 ms | Pass |
| BFS | test_05.txt | 10000 | 30000 | Unweighted adjacency list | 4732 | Traversal / distances | Traversal / distances | 0.632000 ms | Pass |
| DFS | test_05.txt | 10000 | 30000 | Unweighted adjacency list | 4732 | Traversal | Traversal | 0.930000 ms | Pass |
| BFS | test_06.txt | 50000 | 150000 | Unweighted adjacency list | 0 | Traversal / distances | Traversal / distances | 2.524000 ms | Pass |
| DFS | test_06.txt | 50000 | 150000 | Unweighted adjacency list | 0 | Traversal | Traversal | 3.726000 ms | Pass |
| BFS | test_07.txt | 100000 | 300000 | Unweighted adjacency list | 0 | Traversal / distances | Traversal / distances | 4.423000 ms | Pass |
| DFS | test_07.txt | 100000 | 300000 | Unweighted adjacency list | 0 | Traversal | Traversal | 6.223000 ms | Pass |
| SSSP | sssp_test_01.txt | 10 | 30 | Positive-weighted adjacency list | 0 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_02.txt | 100 | 300 | Positive-weighted adjacency list | 0 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_03.txt | 100 | 150 | Positive-weighted adjacency list | 99 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_04.txt | 10000 | 30000 | Positive-weighted adjacency list | 4732 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_05.txt | 10000 | 30000 | Positive-weighted adjacency list | 0 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_06.txt | 50000 | 150000 | Positive-weighted adjacency list | 0 | Shortest distances | Shortest distances |  ms | Pass |
| SSSP | sssp_test_07.txt | 100000 | 300000 | Positive-weighted adjacency list | 0 | Shortest distances | Shortest distances |  ms | Pass |


## Time and Space Complexity

**CSR Conversion:** O(V + E) time (single pass over the input), O(V + E)
space for the three CSR arrays.

**BFS:** O(V + E) time, each vertex is dequeued once, each edge examined once via the CSR neighbor range. O(V) space for the visited array, queue,
and distance array.

**DFS:** O(V + E) time, same reasoning as BFS, each vertex processed once and each edge examined once. O(V + E) space for the explicit stack
(sized to the total edge-entry count as a safe upper bound), plus O(V) for the visited and traversal arrays.

**SSSP (Dijkstra with binary heap):** 
