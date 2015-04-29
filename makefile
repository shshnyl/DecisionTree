all:
	g++ -std=c++11 -o main main.cpp tree.h instance.h

clean:
	rm main
