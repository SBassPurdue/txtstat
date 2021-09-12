.PHONY: txtstat

txtstat:
	g++ -Wall -c -o txtstat.o txtstat.cpp
	g++ -o txtstat txtstat.o

clean:
	rm *.o
