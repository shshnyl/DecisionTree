all:
	g++ -std=c++11 -o main main.cpp main.h tree.h instance.h

clean:
	rm main
