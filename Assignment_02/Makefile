CC = gcc
CFLAGS = -I../common_wrapper -I../utils -Isrc -Idriver

run: ../common_wrapper/wrapper.c ../utils/csr_conversion.c ../utils/graph_helper.c \
     src/triangle_count.c src/betweenness.c src/connected_comp.c \
     driver/triangle_count_runner.c driver/betweenness_runner.c driver/connected_comp_runner.c driver/driver.c
	$(CC) $(CFLAGS) $^ -o run

clean:
	rm -f run
