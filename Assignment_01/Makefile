CC = gcc
CFLAGS = -I../common_wrapper -I../utils -Isrc -Idriver 

SOURCES = ../common_wrapper/wrapper.c \
          ../utils/csr_conversion.c src/bfs.c src/dfs.c src/sssp.c \
          driver/bfs_runner.c driver/dfs_runner.c driver/sssp_runner.c \
          driver/driver.c

run: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o run

clean:
	rm -f run