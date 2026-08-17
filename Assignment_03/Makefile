CC = gcc
CFLAGS = -I../common_wrapper -I../Assingment_01/src -Isrc -Idriver -I../utils

run: ../common_wrapper/wrapper.c \
     src/gradient_descent.c \
     driver/gradient_descent_runner.c driver/driver.c ../utils/csr_conversion.c \
	 src/maxflow_mincut.c driver/maxflow_mincut_runner.c
	$(CC) $(CFLAGS) $^ -o run

clean:
	rm -f run
