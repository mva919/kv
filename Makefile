kv: kv.o parser.o
	cc -o kv kv.o parser.o

kv.o: kv.c
	cc -c kv.c -o kv.o
parser.o: parser.c
	cc -c parser.c -o parser.o

clean:
	rm -f kv kv.o parser.o
