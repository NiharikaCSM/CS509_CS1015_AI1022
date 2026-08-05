# Course Code : CS509 

## About this repository

This repository contains the buddy-task submission for CS509 Assignment 1: BFS, DFS, and SSSP, all implemented on top of a Compressed Sparse Row (CSR) graph representation. Each algorithm reads an adjacency-list input file, converts it to CSR, then runs on the CSR arrays directly. A common wrapper is shared across all assignments to handle repetitive setup like file handling and timing.

## Type: Buddy Assignment

## Student details

- **Student 1:** Niharika Girdhar, 2026CSM1015
- **Student 2:** Vaishali Singh, 2026AIM1022

## Language, compiler, and tools

- **Language:** C
- **Compiler:** gcc 
- **Build tool:** Make 
- **OS tested on:** WSL and macOS 

## Repository structure

```
CS509_CS1015_AI1022/
├── README.md             
├── common_wrapper/
│   ├── wrapper.h
│   └── wrapper.c     
├──utils/    
│   ├── csr_conversion.h
│   └── csr_conversion.c  
├── Assignment_01/
│   ├── Makefile           
│   ├── src/               
│   ├── driver/              
│   ├── tests/               
│   └── readme.md    
├── .gitignore      
```

- **`src/`** contains the actual implementation for that assignment.
- **`driver/`** contains the program that invokes the code in `src/` and runs it against the test cases.
- **`tests/`** contains individual test input files, numbered sequentially. 

## Common wrapper

The `common_wrapper/` folder contains a single wrapper file (`wrapper.c`, with its declarations in `wrapper.h`) reused across all assignments. It handles opening input files and measuring/reporting execution time, so the algorithm code itself stays focused on the algorithm.

## utils
`utils/csr_conversion.c` / `.h` hold the CSR conversion code shared by
BFS, DFS, and SSSP.

**`CSRGraph` struct:**
```c
typedef struct {
    int vertices;
    int edges;
    int *rowPtr;       // size: vertices + 1
    int *colIndex;
    int *edgeWeights;   // NULL if unweighted
} CSRGraph;
```

**Key functions:** `readAdjacencyListAsCSR(fp, isWeighted)` reads an
adjacency-list file and builds the CSR arrays; `readSourceVertex(fp)`
reads the `SOURCE s` line; `freeCSRGraph(graph)` releases memory.
Malformed input (bad header, out-of-range neighbor, negative weight,
etc.) triggers a clear error message and exit — see `tests/bad_*.txt`.

CSR conversion is preprocessing and is never included in any
algorithm's timed execution.

## How to run the code
Each assignment folder has its own `makefile`, so building and running is done **from inside that assignment's folder**, not from the repo root.

At a high level, the general pattern across all assignments is:

```bash
cd Assignment_XX
make
./run <arguments specific to that assignment>
```
 
For the exact command, arguments, and expected output for a specific assignment, refer to that assignment's own `readme.md` (e.g.`Assignment_01/readme.md`). It documents the precise input format, the exact compilation and execution steps, sample test cases, and expected output for that assignment.

To run a different assignment, just `cd` into that assignment's folder and repeat the same steps, each one builds and runs independently.

## Conventions followed

- **Test files** are named `test_01.txt`, `test_02.txt`, etc., numbered in the order they should be run.

- **Runtime measurement** is done inside the common wrapper so that timing is consistent across all assignments — each driver just calls the wrapper's timing function around the actual logic rather than implementing its own.
- Each assignment has its own `readme.md` with any notes specific to that assignment (assumptions made, edge cases handled, how to run it, etc.). This top-level README only covers repo-wide information.