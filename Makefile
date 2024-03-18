spchk: mymalloc.c memgrind.c mymalloc.h
	gcc -o memgrind mymalloc.c memgrind.c

dict: mymalloc.c testcode.c mymalloc.h
	gcc -o testcode mymalloc.c testcode.c

memtest: mymalloc.c memtest.c mymalloc.h
	gcc -o memtest mymalloc.c memtest.c

clean:
	rm -f *.o spchk