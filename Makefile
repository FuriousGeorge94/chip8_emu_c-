chip8: Chip8.cpp
	g++ -g Chip8.cpp -o main -lSDL2

clean:
	rm main a.out
