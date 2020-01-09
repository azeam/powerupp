# change application name here (executable output name)
TARGET=powerupp

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=main.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

clean:
	rm -f *.o $(TARGET)

.PHONY: install
install: powerupp
	mkdir -p /usr/bin
	cp $< /usr/bin/powerupp
	mkdir -p /usr/local/lib/powerupp
	cp lib/upp.py /usr/local/lib/powerupp
	chmod +x /usr/local/lib/powerupp/upp.py
	cp lib/decode.py /usr/local/lib/powerupp
	cp lib/vbios.py /usr/local/lib/powerupp
	cp res/icon.png /usr/share/icons/powerupp.png
	cp powerupp.desktop /usr/share/applications/powerupp.desktop
	cp glade/window_main.glade /usr/local/lib/powerupp/window_main.glade
.PHONY: uninstall
uninstall:
	rm -f /usr/bin/powerupp
	rm -r /usr/local/lib/powerupp
	rm -f /usr/share/icons/powerupp.png
	rm -f /usr/share/applications/powerupp.desktop
