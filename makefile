# application name
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

OBJS=main.o resources.o active.o defaults.o apply.o save.o

ifeq ($(PREFIX),)
    PREFIX := /usr
endif

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

GLIB_COMPILE_RESOURCES = $(shell) $(GIO)

res/resources.c: glade/gio.xml
	$(GLIB_COMPILE_RESOURCES) --target=res/resources.c --sourcedir=glade --generate-source glade/gio.xml

resources.o: res/resources.c
	$(CC) -c $(CCFLAGS) res/resources.c $(GIOLIB) -o resources.o -c

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o

active.o: src/active.c
	$(CC) -c $(CCFLAGS) src/active.c $(GTKLIB) -o active.o

apply.o: src/apply.c
	$(CC) -c $(CCFLAGS) src/apply.c $(GTKLIB) -o apply.o

save.o: src/save.c
	$(CC) -c $(CCFLAGS) src/save.c $(GTKLIB) -o save.o

defaults.o: src/defaults.c
	$(CC) -c $(CCFLAGS) src/defaults.c $(GTKLIB) -o defaults.o

clean:
	rm -f *.o $(TARGET)
	rm -f res/resources.c
.PHONY: install
install: powerupp
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $< $(DESTDIR)$(PREFIX)/bin/powerupp
	install -d $(DESTDIR)$(PREFIX)/share/pixmaps
	install -m 644 res/icon.png $(DESTDIR)$(PREFIX)/share/pixmaps/powerupp.png
	install -d $(DESTDIR)$(PREFIX)/share/applications
	install -m 644 powerupp.desktop $(DESTDIR)$(PREFIX)/share/applications/powerupp.desktop
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/powerupp
	rm -f /usr/bin/powerupp_startup_script_card*.sh
	rm -f $(DESTDIR)$(PREFIX)/share/pixmaps/powerupp.png
	rm -f $(DESTDIR)$(PREFIX)/share/applications/powerupp.desktop
	rm -f /etc/udev/rules.d/80-powerupp*.rules
	# actions below are only to clean up files from older version installations
	rm -f /usr/share/icons/powerupp.png
	rm -f /usr/bin/upp.py
	rm -f /usr/bin/decode.py
	rm -f /usr/bin/vbios.py
	rm -f /etc/systemd/system/powerupp*.service
	systemctl daemon-reload
	systemctl reset-failed
