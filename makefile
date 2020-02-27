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
GIOLIB=`pkg-config --cflags --libs glib-2.0`
GIO=glib-compile-resources

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=main.o resources.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

GLIB_COMPILE_RESOURCES = $(shell) $(GIO)

res/resources.c: glade/gio.xml
	$(GLIB_COMPILE_RESOURCES) --target=res/resources.c --sourcedir=glade --generate-source glade/gio.xml

resources.o: res/resources.c
	$(CC) -c $(CCFLAGS) res/resources.c $(GIOLIB) -o resources.o -c

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

clean:
	rm -f *.o $(TARGET)
	rm -f res/resources.c
.PHONY: install
install: powerupp
	mkdir -p /usr/bin
	cp $< /usr/bin/powerupp
	cp lib/upp.py /usr/bin/upp.py
	chmod +x /usr/bin/upp.py
	cp lib/decode.py /usr/bin/decode.py
	cp lib/vbios.py /usr/bin/vbios.py
	mkdir -p /usr/share/icons
	cp res/icon.png /usr/share/icons/powerupp.png
	mkdir -p /usr/share/applications
	cp powerupp.desktop /usr/share/applications/powerupp.desktop
.PHONY: uninstall
uninstall:
	rm -f /usr/bin/powerupp
	rm -f /usr/bin/upp.py
	rm -f /usr/bin/decode.py
	rm -f /usr/bin/vbios.py
	rm -f /usr/bin/powerupp_startup_script_card*.sh
	rm -f /usr/share/icons/powerupp.png
	rm -f /usr/share/applications/powerupp.desktop
	rm -f /etc/udev/rules.d/80-powerupp*.rules