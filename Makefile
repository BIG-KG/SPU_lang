all: hello

hello:  main.o three_funk.o stackfunk.o canary.o const.o dump.o calculator.o parcer.o dumpTex.o
	g++ main.o three_funk.o stackfunk.o canary.o const.o dump.o calculator.o parcer.o dumpTex.o -o hell.exe

main.o:    main.cpp
	g++ -c main.cpp

three_funk.o: sourse\\three_funk.cpp
	g++ -c    sourse\\three_funk.cpp

dumpTex.o: sourse\\dumpTex.cpp
	g++ -c    sourse\\dumpTex.cpp -I ./headers

stackfunk.o: stack\\stackfunk.cpp
	g++ -c   stack\\stackfunk.cpp

parcer.o:	sourse\\parcer.cpp
	g++ -c  sourse\\parcer.cpp

canary.o:	stack\\canary.cpp
	g++ -c  stack\\canary.cpp

const.o:	stack\\const.cpp
	g++ -c 	stack\\const.cpp

dump.o:    sourse\dump.cpp
	g++ -c sourse\dump.cpp

calculator.o: sourse\calculator.cpp
	g++ -c    sourse\calculator.cpp

clean:
	rm -rf *.o hello2
