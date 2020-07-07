
CC=cl /nologo
CFLAGS=-c -W3 -Ox /MT /wd "4996"
LINKER=link
LINKCON = /nologo /NODEFAULTLIB:libc

all:ftpdmin.exe

OBJS = ftpdmin.obj paths.obj
        

$(OBJS): $(@B).c paths.h
    $(CC) $(CFLAGS) $(@B).c

ftpdmin.exe: $(OBJS)
    $(LINKER) $(LINKCON) -OUT:Bin/ftpdmin.exe $(OBJS) ws2_32.lib 