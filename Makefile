CRESUS=cresus_ex.a
LDFLAGS=-L. -lm

OBJ=	atr.o \
	bollinger.o \
	candle.o \
	esi.o \
	google.o \
	macd.o \
	mobile.o \
	portfolio.o \
	rsi.o \
	statistics.o \
	swing.o \
	yahoo.o \
	zigzag.o
#	xtr.o 

TARGET+=$(CRESUS)
CFLAGS+=--std=gnu99 -Wall

all: $(TARGET)

debug: CFLAGS+=-O0 -g
debug: all

$(CRESUS): $(OBJ)
#	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	$(AR) rcs $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
