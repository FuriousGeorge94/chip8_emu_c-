chip8: Chip8.cpp
	g++ -g main.cpp -o main -lSDL2

clean:
	rm main a.out
