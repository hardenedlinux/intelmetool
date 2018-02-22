CC= gcc
CFLAGS= -Wall -O0 -g
LIBS= -lpci
DESTDIR= /usr/local/bin/

INCLUDES= -I.

SRCS= intelmetool.c \
	  me_status.c \
	  me.c \
	  mmap.c

OBJS = $(SRCS:.c=.o)

TARGET= intelmetool


.PHONY: depend clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

install: 
	cp -t $(DESTDIR) $(TARGET)

clean:
	rm $(TARGET) *.o
