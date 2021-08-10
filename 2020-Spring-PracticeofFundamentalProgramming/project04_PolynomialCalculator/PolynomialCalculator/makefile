main:Polynomial.o Calculator.o UI.o main.o
	g++ Polynomial.o Calculator.o UI.o main.o -o main
main.o:main.cpp
	g++ -c main.cpp -o main.o
Polynomial.o:Polynomial.cpp
	g++ -c Polynomial.cpp -o Polynomial.o
Calculator.o:Calculator.cpp
	g++ -c Calculator.cpp -o Calculator.o
UI.o:UI.cpp
	g++ -c UI.cpp -o UI.o
clean:
	rm *.o
	rm main


