#include <stdio.h>
#include <string.h>
#include "bfs_runner.h"
#include "dfs_runner.h"
#include "sssp_runner.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide input in correct format\n");
        return 1;
    }

    char *filePath = argv[1];
    char *method = argv[2];

    if (strcasecmp(method, "bfs") == 0)
        return runBFSFromFile(filePath);

    if (strcasecmp(method, "dfs") == 0)
         return runDFSFromFile(filePath);

     if (strcasecmp(method, "sssp") == 0)
        return runSSSPFromFile(filePath);

    fprintf(stderr, "Error: Unknown method '%s'. Use 'bfs', 'dfs', or 'sssp'.\n", method);
    return 1;
}
