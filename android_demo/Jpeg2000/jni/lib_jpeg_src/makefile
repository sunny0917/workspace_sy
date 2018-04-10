.PHONY:all clean

BIN  = libopenjpeg2000.so
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
INCS = ../inc/

all:$(BIN)

$(BIN):$(OBJS)
	gcc -o $@ -shared $^

%.o:%.c 
	gcc -o $@ -fPIC -c $^ -I$(INCS)

clean:
	rm -f $(BIN) $(OBJS)


