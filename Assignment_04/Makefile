CC = gcc
CFLAGS = -I../common_wrapper -I../Assingment_01/src -Isrc -Idriver

run: ../common_wrapper/wrapper.c \
     src/fastmap.c \
     driver/fastmap_runner.c driver/driver.c \
     src/kmeans.c driver/kmeans_runner.c
	$(CC) $(CFLAGS) $^ -o run

clean:
	rm -f run