CFLAGS=-I.. --std=gnu99 -Wall
LDFLAGS=../cresus_ex.a -lm

TARGET=	cresus_vp

OBJ=	cresus_vp.o

all: $(TARGET)

debug: CFLAGS+=-O0 -g
debug: all

cresus_%: cresus_%.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
