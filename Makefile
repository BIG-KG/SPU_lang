CC = gcc

CFLAGS = -Wall -g

SRCS = $(wildcard sourse\*.c)

OBJS = $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
 $(CC) $(CFLAGS) -c $< -o $@

clean:
 rm -f $(OBJS)

.PHONY: all clean
