# Assignment 02

**Title:** Graph Algorithms on CSR —  Triangle Counting, Betweenness Centrality, Connected Components

**Type:**  Buddy

**Objective:** Triangle Counting, Betweenness Centrality, and Connected Components (undirected graph analytics), with all three CSR-based algorithms operating on a Compressed Sparse Row representation built from an adjacency-list input file. Each algorithm's execution time is measured separately from setup work (CSR conversion, matrix loading, file I/O).

## Explanation of the Algorithm / Approach

**CSR Conversion** — Reused from Assignment 1 (`readAdjacencyListAsCSR`), not reimplemented here. Applies to Bellman-Ford, Triangle Counting, Betweenness Centrality, and Connected Components, since all four read an adjacency list. Always completed before timing starts.


**Triangle Counting** — For every vertex `u`, examines each pair of neighbors of `u` using the CSR and checks whether that pair is itself connected by an edge (a common-neighbor check). Each triangle is discovered once at each of its three vertices, so the raw count is divided by 3. Neighbor lists are sorted to keep the common-neighbor check fast.

**Betweenness Centrality** — Brandes' algorithm: runs a BFS from every vertex (reusing Assignment 1's BFS logic) and accumulates, for each vertex `v`, the fraction of shortest paths between other pairs that pass through `v`. Raw (unnormalized) values are reported — no division by `(V-1)(V-2)`. O(V·E) time, so the two largest standard graph sizes are reduced (see below).

**Connected Components** — Found via repeated BFS/DFS (reusing Assignment 1's traversal code) starting a new traversal from each unvisited vertex. Every vertex, including isolated ones, is assigned to exactly one component, with component ids numbered in the order each component is first discovered.

## Input Format, Assumptions, and Constraints


**Triangle Counting / Betweenness Centrality / Connected Components** — unweighted, undirected adjacency list:
```
V E
u0 degree neighbor1 neighbor2 ...
...
u(V-1) degree neighbor1 neighbor2 ...
```
No `SOURCE` line for any of these three — Triangle Counting and Connected Components are computed over the whole graph, and Betweenness Centrality uses every vertex as a source internally.

**Assumptions:**
- Vertices are listed in order 0 to V-1.
- Triangle Counting, Betweenness Centrality, and Connected Components graphs are undirected; each edge appears once in each endpoint's adjacency list, and `E` counts each edge once.
- No negative weight is placed on an undirected edge (would form a trivial negative cycle by itself).
- Input files are well-formed (degree counts match the listed neighbors).

**Constraints:**
- The two largest standard graph sizes (50,000 / 100,000) are reduced for Floyd-Warshall (O(V³) time, O(V²) memory) and Betweenness Centrality (O(V·E) via Brandes) — see the required sizes table below.

## Required Graph Sizes

| Algorithm | Required V | Note |
|---|---|---|

| Triangle Counting | 10, 100, 10,000, 50,000, 100,000 | Full standard scale (O(V+E)-adjacent workload) |
| Betweenness Centrality | 10, 100, 1,000, 5,000, 10,000 | Reduced scale — O(V·E) via Brandes |
| Connected Components | 10, 100, 10,000, 50,000, 100,000 | O(V+E), full standard scale |

## Input Validation

Same validation approach as Assignment 1 (`readAdjacencyListAsCSR` / `readSourceVertex`), extended with:

| Condition checked | Example error message |
|---|---|
| Asymmetric Edge| `Invalid input file format - edge (0, 1) is not symmetric: vertex 1's adjacency list does not contain vertex 0, but the graph must be undirected.` |
| Mismatched Degree | `Invalid input file format - edge (0, 1) is not symmetric: vertex 1's adjacency list does not contain vertex 0, but the graph must be undirected.` |
| Duplicate vertex row | `Invalid input file format - vertices must be listed in order 0 to V-1; expected vertex line for a different vertex than the one found.` |
| Edge count mismatch | `matrix entries must be integers or the literal token INF.` |
| Header missing | `Invalid input file format - Number of vertices (V) must be a positive integer` |
| negative Weight present | `Invalid input file format - vertices must be listed in order 0 to V-1; expected vertex line for a different vertex than the one found.` |
| Empty file | `Invalid input file format - file is empty — expected 'V E' on the first line.` |
| Zero vertices | `Invalid input file format - Number of vertices (V) must be a positive integer.` |
| Vertex count mismatched | `Invalid input file format - expected a vertex line (id and degree), but the file ended early or contained non-integer data.` |
| Wrong keyword | ` Invalid input file format - expected a vertex line (id and degree), but the file ended early or contained non-integer data.` |

## Source Files, Driver Files, Helper Functions, and Test Files

**`../utils/csr_conversion.c` / `csr_conversion.h`** — reused unchanged from Assignment 1.


**`src/triangle_count.c` / `triangle_count.h`** — `runTriangleCounting(graph, printTriangles)` returns the total triangle count and (optionally) the list of triangles; `freeTriangleCountingResult(result)` frees them.

**`src/betweenness.c` / `betweenness.h`** — `runBetweennessCentrality(graph)` returns per-vertex raw centrality values; `freeBetweennessCentralityResult(result)` frees them.

**`src/connected_components.c` / `connected_components.h`** — `runConnectedComponents(graph)` returns the component count and per-vertex component id; `freeConnectedComponentsResult(result)` frees them.

**`driver/driver.c`** — parses the input file path and algorithm name (`bf`, `fw`, `tc`, `bc`, `cc`), and dispatches to the matching runner.

**`tc_runner.c`, `bc_runner.c`, `cc_runner.c`** — each opens the input file, prepares the required data structure (CSR or dense matrix, untimed), times only the algorithm call, prints the result, and frees all allocated memory.

**`../common_wrapper/wrapper.c`** (shared across all assignments) — `openFile`, `getExecutionTime`, `printExecutionTime`, reused unchanged.

**`tests/`** — adjacency-list / matrix input files at the required sizes, generated and verified against reference implementations (networkx / scipy.sparse.csgraph):
- `tc_<V>.txt` for V in {10, 100, 10000, 50000, 100000}, plus edge cases (`tc_no_edges`, `tc_path8`, `tc_cycle10`, `tc_single_triangle`, `tc_complete5`, `tc_star7`, `tc_disconnected`) and `tc_example.txt` (spec §7.2)
- `bc_<V>.txt` for V in {10, 100, 1000, 5000, 10000}, plus edge cases (`bc_single`, `bc_no_edges`, `bc_star7`, `bc_path8`, `bc_cycle8`, `bc_disconnected`, `bc_complete6`) and `bc_example.txt` (spec §8.2)
- `cc_<V>.txt` for V in {10, 100, 10000, 50000, 100000}, plus `cc_example.txt` (spec §9.2)
- `*_expected.txt` files with verified expected output for every hand-checkable / small-to-medium case

## Compilation and Execution Instructions

From inside `Assignment_02/`:

**Compile:**
```bash
make
```
**Run Malformed Input test cases:**
```bash
./run tests/malformed_test/mi_<test file name>.txt tc
./run tests/malformed_test/mi_<test file name>.txt bc
./run tests/malformed_test/mi_<test file name>.txt cc
```



**Run each algorithm:**
```bash

./run tests/tc_10.txt tc
./run tests/bc_10.txt bc
./run tests/cc_10.txt cc
```

**Clean up the compiled binary:**
```bash
make clean
```


## Results Table — Graph Analytics

| Algorithm | Test File | Vertices | Edges | Time | Status |
|---|---|---|---|---|---|
| Triangle Counting | tc_test_01.txt | 10 | 20 | 0.003000 ms | Completed Successfully |
| Triangle Counting | tc_test_02.txt | 100 | 250 | 0.085000 ms | Completed Successfully |
| Triangle Counting | tc_test_03.txt | 10,000 | 20,000 | 1.128000 ms | Completed Successfully |
| Triangle Counting | tc_test_04.txt | 50,000 | 100,000 | 4.623000 ms | Completed Successfully |
| Triangle Counting | tc_test_05.txt | 100,000 | 200,000 | 8.828000 ms | Completed Successfully |
| Betweenness Centrality | bc_test_01.txt | 10 | 6 | 0.006000 ms | Completed Successfully |
| Betweenness Centrality | bc_test_02.txt | 100 | 200 | 0.674000 ms | Completed Successfully |
| Betweenness Centrality | bc_test_03.txt | 1,000 | 2,000 | 73.480000 ms | Completed Successfully |
| Betweenness Centrality | bc_test_04.txt | 5,000 | 10,000 | 2172.461000 ms | Completed Successfully |
| Betweenness Centrality | bc_test_05.txt | 10,000 | 20,000 | 9973.888000 ms | Completed Successfully |
| Connected Components | cc_test_01.txt | 10 | 9 | 0.002000 ms | Completed Successfully |
| Connected Components | cc_test_02.txt | 100 | 167 | 0.008000 ms | Completed Successfully |
| Connected Components | cc_test_03.txt | 10,000 | 17,615 | 0.433000 ms | Completed Successfully |
| Connected Components | cc_test_04.txt | 50,000 | 88,153 | 2.817000 ms | Completed Successfully |
| Connected Components | cc_test_05.txt | 100,000 | 176,325 | 6.148000 ms | Completed Successfully |

## Time and Space Complexity


**Triangle Counting:** O(V·d_max²) in the worst case (examining every pair of neighbors per vertex and checking `hasEdge`), improved by sorting neighbor lists for faster common-neighbor checks. O(V+E) space for the CSR graph plus O(T) for the optional triangle list.

**Betweenness Centrality (Brandes'):** O(V·E) time — one BFS per source vertex, each O(V+E). O(V+E) space per source for the BFS/backward-accumulation structures (distance, sigma, delta, predecessor lists), reused across sources.

**Connected Components:** O(V+E) time — each vertex and edge visited once across all BFS/DFS traversals. O(V) space for the visited/component-id array plus O(V+E) for the traversal structures.
