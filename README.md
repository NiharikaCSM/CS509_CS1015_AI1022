# Course Code : CS509 

## About this repository

This repository contains the buddy-task submission for CS509 Assignment 1: BFS, DFS, and SSSP, all implemented on top of a Compressed Sparse Row (CSR) graph representation. Each algorithm reads an adjacency-list input file, converts it to CSR, then runs on the CSR arrays directly. A common wrapper is shared across all assignments to handle repetitive setup like file handling and timing.

## Student details

- **Student 1:** Niharika Girdhar, 2026CSM1015
- **Student 2:** Vaishali Singh, 2026AIM1022

## Language, compiler, and tools

- **Language:** C
- **Compiler:** gcc 
- **Build tool:** Make 
- **OS tested on:** Windows and macOS 

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
├── assignment_01/
│   ├── Makefile           
│   ├── src/               
│   ├── driver/              
│   ├── tests/               
│   └── readme.md          
```

- **`src/`** 
- **`driver/`** 
- **`tests/`** 

## Common wrapper

The `common_wrapper/` folder contains a single wrapper file (`wrapper.c`, with its declarations in `wrapper.h`) reused across all assignments. It handles opening input files and measuring/reporting execution time, so the algorithm code itself stays focused on the algorithm.

## utils

## How to run the code
(to be filled)

## Conventions followed
(to be filled)

