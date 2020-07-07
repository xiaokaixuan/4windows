
CC=cl /nologo
CFLAGS=-c -W3 -Ox /MT
LINKER=link
LINKCON = /nologo /NODEFAULTLIB:libc

all:ftpdmin.exe

OBJS = ftpdmin.obj paths.obj
        

$(OBJS): $(@B).c paths.h
    $(CC) $(CFLAGS) $(@B).c

ftpdmin.exe: $(OBJS)
    $(LINKER) $(LINKCON) -OUT:ftpdmin.exe $(OBJS) ws2_32.lib 