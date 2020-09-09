all: virtual-instrument

virtual-instrument: main.cpp
	g++ main.cpp -o virtual-instrument -llo

.PHONY: run clean

run: virtual-instrument
	./virtual-instrument

clean:
	rm -f virtual-instrument