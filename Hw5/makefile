
mandelbrot:
	mkdir -p bin
	g++ --std=c++17 -O2 -Wall -o bin/mandelbrot src/mandelbrot.cpp src/task.cpp src/threadpool.cpp src/mandelbrot_helper.cpp -lm
e0:
	bin/mandelbrot -2 1 -1.5 1.5 e0.bmp

e1:
	bin/mandelbrot -0.5 0 0.3 1.2 e1.bmp

e2:
	bin/mandelbrot 0.3 0.4 0.6 0.7 e2.bmp

e3:
	bin/mandelbrot -0.2 0.0 -1.0 -0.9 e3.bmp

e4:
	bin/mandelbrot -0.05 -0.01 -1.01 -0.97 e4.bmp
all: mandelbrot e0 e1 e2 e3 e4

clean:
	rm -rf bin *.bmp