main:menu.o func_cp.o func_cmp.o func_cat.o func_wc.o func_man.o func_more.o main.o
	g++ menu.o func_cp.o func_cmp.o func_cat.o func_wc.o func_man.o func_more.o main.o -o main -l curses
main.o:main.cpp
	g++ -c main.cpp -o main.o
menu.o:menu.cpp
	g++ -c menu.cpp -o menu.o
func_cp.o:func_cp.cpp
	g++ -c func_cp.cpp -o func_cp.o
func_cmp.o:func_cmp.cpp
	g++ -c func_cmp.cpp -o func_cmp.o
func_cat.o:func_cat.cpp
	g++ -c func_cat.cpp -o func_cat.o
func_wc.o:func_wc.cpp
	g++ -c func_wc.cpp -o func_wc.o
func_man.o:func_man.cpp
	g++ -c func_man.cpp -o func_man.o
func_more.o:func_more.cpp
	g++ -c func_more.cpp -o func_more.o
clean:
	rm *.o
	rm main
