SRCS = $(wildcard sourse/*.cpp)

OBJS = $(SRCS:.cpp=.o)

all: hello 

hello: $(OBJS) main.o
	g++ main.o $(OBJS) -o hell.exe

main.o: main.cpp
	g++ -c main.cpp -I ./headers 

%.o: %.cpp
	g++  -c $< -o $@ -I ./headers

clean:
	DEL *.o
	DEL *.exe